find_package(Boost COMPONENTS unit_test_framework REQUIRED)

include(CTest)
enable_testing()

#sheet1_2: test_vector
add_executable(test_vector test_vec.cpp)
target_link_libraries(test_vector PRIVATE Boost::unit_test_framework MDlib)
add_test (NAME TestVector COMMAND test_vector)

#sheet1_2: test_particle
add_executable(test_particle test_particle.cpp)
target_link_libraries(test_particle PRIVATE Boost::unit_test_framework MDlib)
add_test (NAME TestParticle COMMAND test_particle)

#sheet3: Exercise 1
add_executable(test_particlecontainer test_particlecontainer.cpp)
target_link_libraries(test_particlecontainer PRIVATE Boost::unit_test_framework MDlib)
add_test (NAME TestParticlecontainer COMMAND test_particlecontainer)

#sheet3: Exercise 2
add_executable(test_aos_forces test_aos_forces.cpp)
target_link_libraries(test_aos_forces PRIVATE Boost::unit_test_framework MDlib)
add_test (NAME TestAoSForce COMMAND test_aos_forces)

#sheet3: Exercise 3
add_executable(test_aos_integrator test_aos_integrator.cpp)
target_link_libraries(test_aos_integrator PRIVATE Boost::unit_test_framework MDlib)
add_test (NAME TestAoSIntegrator COMMAND test_aos_integrator)

#sheet3: Exercise 4
add_executable(test_aos_boundaries test_aos_boundaries.cpp)
target_link_libraries(test_aos_boundaries PRIVATE Boost::unit_test_framework MDlib)
add_test (NAME TestAoSBoundaries COMMAND test_aos_boundaries)

#sheet4: Exercise 2.1
add_executable(test_soa_container test_soa_container.cpp)
target_link_libraries(test_soa_container PRIVATE Boost::unit_test_framework MDlib)
add_test (NAME TestSaOContainer COMMAND test_soa_container)

#sheet4: Exercise 2.2
add_executable(test_soa_force test_soa_forces.cpp)
target_link_libraries(test_soa_force PRIVATE Boost::unit_test_framework MDlib)
add_test (NAME TestSoAForce COMMAND test_soa_force)

#sheet4: Exercise 2.3
add_executable(test_soa_boundaries test_soa_boundaries.cpp)
target_link_libraries(test_soa_boundaries PRIVATE Boost::unit_test_framework MDlib)
add_test (NAME TestSoABoundaries COMMAND test_soa_boundaries)

#sheet4: Exercise 2.4
add_executable(test_soa_integrator test_soa_integrator.cpp)
target_link_libraries(test_soa_integrator PRIVATE Boost::unit_test_framework MDlib)
add_test (NAME TestSoAIntegrator COMMAND test_soa_integrator)

#sheet5: Exercise 2
add_executable(test_linked_cell test_linkedcellcontainer.cpp)
target_link_libraries(test_linked_cell PRIVATE Boost::unit_test_framework MDlib)
add_test (NAME TestLinkedCell COMMAND test_linked_cell)

##sheet5: Exercise 3
add_executable(test_lc_forces test_linked_cell_forces.cpp)
target_link_libraries(test_lc_forces PRIVATE Boost::unit_test_framework MDlib)
add_test (NAME TestLCForces COMMAND test_lc_forces)
