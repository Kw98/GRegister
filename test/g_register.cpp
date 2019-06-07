#include <iostream>

#include "../src/GRegister.hpp"

struct s {
      int   x;
      int   y;
      /* data */
};

struct s2 {
      std::string name;
      int         age;
      int         size;
      /* data */
};


struct hitbox {
      int   id;
};

struct circle : public hitbox
{
      int pi;
      /* data */
};

struct oval : public hitbox
{
      int x;
      int y;
      /* data */
};


int   main() {
      g_reg::GRegister  reg(100, 2);

      std::cout << "test create entity:" << std::endl;
      for (int i = 0; i < 100; i++) {
            auto id = reg.create();
            std::cout << "id: " << id << std::endl;
            reg.construct<s>(id, i, i);
            s2    st = {std::string("wow"), 25, i};
            reg.assign(id, st);
      }

      std::cout << "test delete components:" << std::endl;
      for (std::size_t i = 30; i < 50; i++)
            reg.delete_components<s2>(i);
      for (std::size_t i = 70; i < 100; i++)
            reg.delete_components<s>(i);

      std::cout << "test view / safe_view:" << std::endl;
      for (std::size_t i = 0; i < 30; i++) {
            auto v = reg.view<s, s2>(i);
            std::cout << "v1: " << std::get<s>(v).x << " " << std::get<s>(v).y << " ";
            std::cout << std::get<s2>(v).name << " " << std::get<s2>(v).size << " " << std::get<s2>(v).age << std::endl;
            std::get<s2>(v).name = std::string("A9UZOQJSM");
      }
      for (std::size_t i = 30; i < 50; i++) {
            auto v = reg.safe_view<s>(i);
            std::cout << "v2: " << std::get<s *>(v)->x << " " << std::get<s *>(v)->y << " " << std::endl;
      }
      for (std::size_t i = 50; i < 70; i++) {
            auto v = reg.safe_view<s, s2>(i);
            std::cout << "v3: " << std::get<s *>(v)->x << " " << std::get<s *>(v)->y << " ";
            std::cout << std::get<s2 *>(v)->name << " " << std::get<s2 *>(v)->size << " " << std::get<s2 *>(v)->age << std::endl;
      }
      for (std::size_t i = 70; i < 100; i++) {
            auto v = reg.safe_view<s2>(i);
            std::cout << "v4: ";
            std::cout << std::get<s2 *>(v)->name << " " << std::get<s2 *>(v)->size << " " << std::get<s2 *>(v)->age << std::endl;
      }


      std::cout << "test cpool_has: " << std::endl;
      std::cout << "should have: " << reg.cpool_has<s, s2>(0) << std::endl;
      std::cout << "should not have: " << reg.cpool_has<int>(0) << std::endl;
      std::cout << "should not have: " << reg.cpool_has<s, s2>(40) << std::endl;
      std::cout << "should not have: " << reg.cpool_has<s, s2>(70) << std::endl;
      std::cout << "should have: " << reg.cpool_has<s>(40) << std::endl;
      std::cout << "should have: " << reg.cpool_has<s2>(70) << std::endl;

      std::cout << "test global_view:" << std::endl;
      auto gv1 = reg.global_view<s, s2>();
      for (auto &&tuple : gv1) {
            std::cout << "gv1: " << std::get<s *>(tuple.components)->x << " " << std::get<s *>(tuple.components)->y << " ";
            std::cout << std::get<s2 *>(tuple.components)->name << " " << std::get<s2 *>(tuple.components)->size << " " << std::get<s2 *>(tuple.components)->age << std::endl;
      }
      
      std::cout << "test with inheritance: "  << std::endl;
      hitbox      h_1 = {1};
      oval        h_2 = {2, 3231, 4131};
      circle      h_3 = {8, 13};
      std::cout << "-- before: "  << std::endl;
      std::cout << "hitbox: " << " id= " << h_1.id << std::endl;
      std::cout << "oval: " << " id= " << h_2.id << " x= " << h_2.x << " y= " << h_2.y << std::endl;
      std::cout << "circle: " << " id= " << h_3.id << " pi= " << h_3.pi << std::endl;
      reg.assign(1, h_1);
      reg.assign(2, h_2);
      reg.assign(3, h_3);
      auto h_1_1 = reg.try_get<hitbox>(1);
      auto h_2_1 = reg.try_get<oval>(2);
      auto h_3_1 = reg.try_get<circle>(3);
      std::cout << "-- after: "  << std::endl;
      std::cout << "hitbox: " << " id= " << h_1_1->id << std::endl;
      if (h_2_1)
            std::cout << "oval: " << " id= " << h_2_1->id << ", x= " << h_2_1->x << ", y= " << h_2_1->y << std::endl;
      if (h_3_1)
            std::cout << "circle: " << " id= " << h_3_1->id << ", pi= " << h_3_1->pi <<  std::endl;
      return 0;
}