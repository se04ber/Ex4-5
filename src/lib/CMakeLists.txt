find_package(MPI REQUIRED)

add_library(MDlib SHARED)
target_compile_options(MDlib PUBLIC -Werror -O3 -march=native -fopenmp)

#sheet 1_2 -  Exercise 2
target_sources(MDlib PUBLIC vector3d.cpp)

#sheet 1_2 - Exercise 3
target_sources(MDlib PUBLIC particle3d.cpp)

#sheet 3 - Exercise 1
target_sources(MDlib PUBLIC particlecontainer.cpp)
#sheet 3 - Exercise 2
target_sources(MDlib PUBLIC forces.cpp)
#sheet 3 - Exercise 3
target_sources(MDlib PUBLIC integrator.cpp)
#sheet 3 - Exercise 4
target_sources(MDlib PUBLIC boundaries.cpp)

#sheet 4 - Exercise 1
target_sources(MDlib PUBLIC problem.cpp)
#sheet 4 - Exercise 2
target_sources(MDlib PUBLIC utils.cpp)
target_sources(MDlib PUBLIC soacontainer.cpp)

##sheet 5 - Exercise 2
target_sources(MDlib PUBLIC linkedcellcontainer.cpp)

##sheet 5 - Exercise 4
#target_sources(MDlib PUBLIC problem.cpp)

#target_link_options(MDlib PUBLIC -fopenmp)
#target_compile_definitions(MDlib PUBLIC -DNDEBUG)
