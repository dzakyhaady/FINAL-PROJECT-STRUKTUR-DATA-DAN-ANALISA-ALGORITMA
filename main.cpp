#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <memory>

/* ================= KONSTANTA ================= */

constexpr unsigned WIDTH = 800;
constexpr unsigned HEIGHT = 600;

constexpr float MIN_RADIUS = 6.f;
constexpr float MAX_RADIUS = 18.f;

constexpr float SPEED = 140.f;
constexpr float COLLISION_FORCE = 18.f;

/* drag sangat ringan */
constexpr float DRAG = 0.25f;   // makin kecil = makin licin
constexpr float STOP_EPS = 1.5f;

/* ================= BALL ================= */

struct Ball {
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float radius;

    Ball(sf::Vector2f pos) {
        radius = MIN_RADIUS +
            static_cast<float>(rand()) / RAND_MAX *
            (MAX_RADIUS - MIN_RADIUS);

        shape.setRadius(radius);
        shape.setOrigin({radius, radius});
        shape.setPosition(pos);
        shape.setFillColor(sf::Color(
            rand() % 255,
            rand() % 255,
            rand() % 255
        ));

        velocity = {
            (rand() % 200 - 100) / 100.f * SPEED,
            (rand() % 200 - 100) / 100.f * SPEED
        };
    }

    void update(float dt) {
        /* gerak */
        shape.move(velocity * dt);

        /* ===== GRAVITASI TIPIS (DRAG) ===== */
        velocity -= velocity * DRAG * dt;

        if (std::hypot(velocity.x, velocity.y) < STOP_EPS)
            velocity = {0.f, 0.f};

        auto p = shape.getPosition();

        if (p.x < radius || p.x > WIDTH - radius)
            velocity.x *= -1;
        if (p.y < radius || p.y > HEIGHT - radius)
            velocity.y *= -1;
    }
};


void resolveCollision(Ball& a, Ball& b) {
    sf::Vector2f d = b.shape.getPosition() - a.shape.getPosition();
    float dist = std::hypot(d.x, d.y);
    float minDist = a.radius + b.radius;

    if (dist == 0 || dist > minDist)
        return;

    sf::Vector2f n = d / dist;

    a.velocity -= n * COLLISION_FORCE;
    b.velocity += n * COLLISION_FORCE;
}


void bruteforceCollision(std::vector<Ball>& balls) {
    for (size_t i = 0; i < balls.size(); ++i)
        for (size_t j = i + 1; j < balls.size(); ++j)
            resolveCollision(balls[i], balls[j]);
}


struct Quadtree {
    sf::FloatRect boundary;
    std::vector<Ball*> objects;
    bool divided = false;

    std::unique_ptr<Quadtree> nw, ne, sw, se;
    static constexpr int CAPACITY = 4;

    Quadtree(sf::FloatRect area) : boundary(area) {}

    void subdivide() {
        auto p = boundary.position;
        auto s = boundary.size / 2.f;

        nw = std::make_unique<Quadtree>(sf::FloatRect(p, s));
        ne = std::make_unique<Quadtree>(sf::FloatRect({p.x + s.x, p.y}, s));
        sw = std::make_unique<Quadtree>(sf::FloatRect({p.x, p.y + s.y}, s));
        se = std::make_unique<Quadtree>(sf::FloatRect(p + s, s));

        divided = true;
    }

    void insert(Ball* b) {
        if (!boundary.contains(b->shape.getPosition()))
            return;

        if (objects.size() < CAPACITY)
            objects.push_back(b);
        else {
            if (!divided)
                subdivide();
            nw->insert(b);
            ne->insert(b);
            sw->insert(b);
            se->insert(b);
        }
    }

    void query(const sf::FloatRect& range, std::vector<Ball*>& found) {
        if (!boundary.findIntersection(range))
            return;

        for (auto* b : objects)
            if (range.contains(b->shape.getPosition()))
                found.push_back(b);

        if (divided) {
            nw->query(range, found);
            ne->query(range, found);
            sw->query(range, found);
            se->query(range, found);
        }
    }
};


int main() {
    sf::RenderWindow window(
        sf::VideoMode({WIDTH, HEIGHT}),
        "Simulasi Bruteforce vs Quadtree"
    );

    sf::Clock clock;
    std::vector<Ball> balls;
    bool useQuadtree = false;

    sf::Font font;
    font.openFromFile("C:/Users/dzaky/OneDrive/Desktop/project/DejaVuSans.ttf");

    sf::Text modeText(font);
    modeText.setCharacterSize(16);
    modeText.setPosition({10.f, 10.f});

    sf::Text fpsText(font);
    fpsText.setCharacterSize(16);
    fpsText.setPosition({10.f, 30.f});
    fpsText.setFillColor(sf::Color::Yellow);

    int frameCounter = 0;
    float timeAccumulator = 0.f;
    float fps1000 = 0.f;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        frameCounter++;
        timeAccumulator += dt;
        if (frameCounter == 1000) {
            fps1000 = 1000.f / timeAccumulator;
            frameCounter = 0;
            timeAccumulator = 0.f;
        }

        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (event->is<sf::Event::KeyPressed>()) {
                auto k = event->getIf<sf::Event::KeyPressed>();
                if (k->code == sf::Keyboard::Key::Tab)
                    useQuadtree = !useQuadtree;
            }

            if (event->is<sf::Event::MouseButtonPressed>()) {
                auto m = event->getIf<sf::Event::MouseButtonPressed>();
                if (m->button == sf::Mouse::Button::Left)
                    balls.emplace_back(sf::Vector2f(m->position));
            }
        }

        if (useQuadtree) {
            Quadtree qt(sf::FloatRect({0, 0}, {WIDTH, HEIGHT}));
            for (auto& b : balls)
                qt.insert(&b);

            for (auto& b : balls) {
                std::vector<Ball*> near;
                float r = b.radius * 2.f;
                sf::FloatRect range(
                    b.shape.getPosition() - sf::Vector2f(r, r),
                    {r * 2, r * 2}
                );
                qt.query(range, near);

                for (auto* o : near)
                    if (&b != o)
                        resolveCollision(b, *o);
            }
        } else {
            bruteforceCollision(balls);
        }

        for (auto& b : balls)
            b.update(dt);

        modeText.setString(useQuadtree ? "MODE: QUADTREE" : "MODE: BRUTEFORCE");
        modeText.setFillColor(useQuadtree ? sf::Color::Green : sf::Color::Red);
        fpsText.setString("FPS (1000f): " + std::to_string((int)fps1000));

        window.clear(sf::Color::Black);
        for (auto& b : balls)
            window.draw(b.shape);
        window.draw(modeText);
        window.draw(fpsText);
        window.display();
    }

    return 0;
}
