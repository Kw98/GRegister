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
            std::cout << "v1: " << std::get<s &>(v).x << " " << std::get<s &>(v).y << " ";
            std::cout << std::get<s2 &>(v).name << " " << std::get<s2 &>(v).size << " " << std::get<s2 &>(v).age << std::endl;
            std::get<s2 &>(v).name = std::string("A9UZOQJSM");
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
      return 0;
}