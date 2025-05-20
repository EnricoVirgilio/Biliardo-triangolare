#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
namespace pf {
struct Parameters {
  float r1;
  float r2;
  float l;
};
struct Origin {
  const float O_x{50.f};
  const float O_y{300.f};
};

class Pool {
  Origin o_;
  sf::RenderWindow window_;
  Parameters& p_;

 public:
  Pool(Parameters& p);
  sf::RenderWindow& get_window();
  void draw_window();
  void draw_axis();
  void draw_walls();
  void change_parameters(Parameters p);
};

class RangeInput {
  float yLine_;
  float xMinLine_;
  float length_{100.f};
  bool isActive_;
  sf::CircleShape circle_;
  sf::RenderWindow& window_;
  Parameters& p_;
  Pool& pool_;

 public:
  RangeInput(float yLine, float xMinLine, sf::RenderWindow& window,
             Parameters& p, Pool& pool)
      : yLine_{yLine},
        xMinLine_{xMinLine},
        window_{window},
        p_{p},
        pool_{pool}
        {}
  void draw_line();
  void draw_circle();
  void set_initial_circle(float max_parameter, std::string parameter, sf::Color color);
  void move_circle(sf::Event event, float max_parameter, std::string parameter);
  void make_active(sf::Event event);
  void no_active();
  bool is_active();
  void move_r2();
};
}  // namespace pf