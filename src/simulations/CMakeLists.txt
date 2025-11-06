# physical simulation
add_executable(mdsimulation mdsimulation.cpp)
target_compile_options(mdsimulation PUBLIC -Wall -O3 -march=native -fopenmp)
target_link_libraries(mdsimulation MDlib)
target_link_options(mdsimulation PUBLIC -fopenmp)

###### FOR BONUS TASK ##########

# minimalized simulation to profile using valgrind
#add_executable(valgrind_target profiling.cpp)
#target_compile_options(valgrind_target PUBLIC -g -Wall -O3 -march=native -fopenmp)
#target_link_libraries(valgrind_target MDlib)
#target_link_options(valgrind_target PUBLIC -fopenmp)
# minimalized simulation to profile using gprof
#add_executable(gprof_target profiling.cpp)
#target_compile_options(gprof_target PUBLIC -Wall -g -pg -Og -march=native -fopenmp)
#target_link_libraries(gprof_target MDlib)
#target_link_options(gprof_target PUBLIC -fopenmp)
