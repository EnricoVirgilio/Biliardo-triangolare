#include <SFML/Graphics.hpp>
#include "pool2.hpp"
#include <SFML/Config.hpp>
#include <array>
#include <cmath>
#include <limits>
#include <string>

#include "iostream"
namespace pf {
Pool::Pool(Parameters& p)
    : window_{sf::VideoMode({1000, 600}), "SFML window"}, p_{p} {}
void Pool::draw_window() {
  RangeInput r1{100.f, 850.f, window_, p_, *this};
  RangeInput r2{300.f, 850.f, window_, p_, *this};
  RangeInput l{500.f, 850.f, window_, p_, *this};
  r1.set_initial_circle(200.f, "r1", sf::Color::Red);
  r2.set_initial_circle(p_.r1, "r2", sf::Color::Green);
  l.set_initial_circle(750.f, "l", sf::Color::Yellow);
  while (window_.isOpen()) {
    sf::Event event;
    while (window_.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window_.close();  // chiude la finestra
      }
      if (event.type == sf::Event::MouseButtonPressed &&
          event.mouseButton.button == sf::Mouse::Left) {
        r1.make_active(event);
        r2.make_active(event);
        l.make_active(event);
      }
      if (event.type == sf::Event::MouseMoved &&
          sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        bool isactive = r1.is_active();
        if (isactive) {
          r1.move_circle(event, 200.f, "r1");
          r2.move_r2();
        }
        isactive = r2.is_active();
        if (isactive && p_.r1 != 0.f) {
          r2.move_circle(event, p_.r1, "r2");
        }
        isactive = l.is_active();
        if (isactive) {
          l.move_circle(event, 750.f, "l");
        }
      }
      if (event.type == sf::Event::MouseButtonReleased &&
          event.mouseButton.button == sf::Mouse::Left) {
        r1.no_active();
        r2.no_active();
        l.no_active();
      }
    }
    window_.clear();
    draw_axis();
    draw_walls();
    r1.draw_line();
    r2.draw_line();
    l.draw_line();
    r1.draw_circle();
    r2.draw_circle();
    l.draw_circle();
    window_.display();
  }
}
void Pool::draw_axis() {
  std::array y_axis{sf::Vertex{sf::Vector2f(o_.O_x, 100.f)},
                    sf::Vertex{sf::Vector2f(o_.O_x, 500.f)}};
  window_.draw(y_axis.data(), y_axis.size(), sf::PrimitiveType::Lines);

  std::array x_axis{sf::Vertex{sf::Vector2f(30.f, o_.O_y)},
                    sf::Vertex{sf::Vector2f(800.f, o_.O_y)}};
  window_.draw(x_axis.data(), x_axis.size(), sf::PrimitiveType::Lines);
}
void Pool::draw_walls() {
  std::array upper_wall{
      sf::Vertex{sf::Vector2f(o_.O_x, o_.O_y - p_.r1)},
      sf::Vertex{sf::Vector2f(o_.O_x + p_.l, o_.O_y - p_.r2)}};
  window_.draw(upper_wall.data(), upper_wall.size(), sf::PrimitiveType::Lines);

  std::array lower_wall{
      sf::Vertex{sf::Vector2f(o_.O_x, o_.O_y + p_.r1)},
      sf::Vertex{sf::Vector2f(o_.O_x + p_.l, o_.O_y + p_.r2)}};
  window_.draw(lower_wall.data(), lower_wall.size(), sf::PrimitiveType::Lines);
}

void RangeInput::draw_line() {
  std::array input_line{sf::Vertex{sf::Vector2f(xMinLine_, yLine_)},
                        sf::Vertex{sf::Vector2f(xMinLine_ + length_, yLine_)}};
  window_.draw(input_line.data(), input_line.size(), sf::PrimitiveType::Lines);
}
void RangeInput::draw_circle() { window_.draw(circle_); }
void RangeInput::set_initial_circle(float max_parameter, std::string parameter,
                                    sf::Color color) {
  circle_.setRadius(10.f);
  float r = circle_.getRadius();
  if (parameter == "r1") {
    circle_.setPosition(
        {xMinLine_ + (p_.r1 / max_parameter) * length_ - r, yLine_ - r});
  } else if (parameter == "r2") {
    circle_.setPosition(
        {xMinLine_ + (p_.r2 / max_parameter) * length_ - r, yLine_ - r});
  } else if (parameter == "l") {
    circle_.setPosition(
        {xMinLine_ + (p_.l / max_parameter) * length_ - r, yLine_ - r});
  }
  circle_.setFillColor(color);
}
void RangeInput::move_circle(sf::Event event, float max_parameter,
                             std::string parameter) {
  float x_mouse = static_cast<float>(event.mouseMove.x);
  float r = circle_.getRadius();
  float x_centro;
  float y_centro = yLine_;
  if (x_mouse < xMinLine_) {
    x_centro = xMinLine_;
  } else if (x_mouse > xMinLine_ + length_) {
    x_centro = xMinLine_ + length_;
  } else {
    x_centro = x_mouse;
  }

  circle_.setPosition({x_centro, y_centro});
  float pixel_pos = circle_.getPosition().x - xMinLine_ +
                    r;  // restituisce la distanza in pixel del centro del
                        // cerchio rispetto a xMinLine_
  float new_parameter =
      (pixel_pos / length_) *
      max_parameter;  // calcola il valore di r1 corrispondente a pixel_pos
  if (parameter == "r1") {
    p_.r1 = new_parameter;
  } else if (parameter == "r2") {
    p_.r2 = new_parameter;
  } else if (parameter == "l") {
    p_.l = new_parameter;
  }
}
void RangeInput::make_active(sf::Event event) {
  float x_mouse = static_cast<float>(event.mouseButton.x);
  float y_mouse = static_cast<float>(event.mouseButton.y);
  float r = circle_.getRadius();
  float x_centro = circle_.getPosition().x + r;
  float y_centro = circle_.getPosition().y + r;
  float distanza_mc = sqrtf((x_mouse - x_centro) * (x_mouse - x_centro) +
                            (y_mouse - y_centro) * (y_mouse - y_centro));
  if (distanza_mc > r)
    isActive_ = false;
  else {
    isActive_ = true;
  }
}
void RangeInput::no_active() { isActive_ = false; }
bool RangeInput::is_active() { return isActive_; }
void RangeInput::move_r2() {
  float r = circle_.getRadius();
  if (p_.r1 < p_.r2) {
    p_.r2 = p_.r1;
  }
  if (p_.r1 == 0.f) {
    circle_.setPosition({(xMinLine_ + length_ / 2) - r, yLine_ - r});
  } else {
    circle_.setPosition(
        {xMinLine_ + (p_.r2 / p_.r1) * length_ - r, yLine_ - r});
  }
}
}  // namespace pf