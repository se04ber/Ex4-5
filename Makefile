# absolute path of this makefile
# (the top-level directory of the project)
SCRIPTPATH := $(shell dirname $(abspath $(lastword $(MAKEFILE_LIST))))

.PHONY: shell container clean format build test

all: clean container format build test

shell:
	# open interactive command line in container
	cd $(SCRIPTPATH)
	apptainer shell $(SCRIPTPATH)/container/container.sif

container:
	# build container
	rm -f $(SCRIPTPATH)/container/container.sif
	apptainer build $(SCRIPTPATH)/container/container.sif $(SCRIPTPATH)/container/container.def

clean:
	# remove all output files
	rm -rf ./simulation_results/mdsimulation/*
	rm -rf ./simulation_results/particlescaling/*
	rm -rf ./simulation_results/profiling/*
	rm -rf ./simulation_results/strongscaling/*
	rm -rf ./simulation_results/strongscaling/*
	rm -rf $(SCRIPTPATH)/out/*

cleancontainer:
	rm -rf $(SCRIPTPATH)/container/container.sif

build:
	# build all CMake targets
	rm -rf ./out/*
	cd $(SCRIPTPATH)
	apptainer exec $(SCRIPTPATH)/container/container.sif cmake -B out/ -S src/
	apptainer exec $(SCRIPTPATH)/container/container.sif cmake --build out/ --parallel

test:
	# perform CTest tests
	apptainer exec $(SCRIPTPATH)/container/container.sif ctest --rerun-failed --output-on-failure --test-dir out/tests

filtertests:
	apptainer exec $(SCRIPTPATH)/container/container.sif pytest src/filtering/

format:
	# format code
	cd $(SCRIPTPATH)
	apptainer exec $(SCRIPTPATH)/container/container.sif clang-format --style=Google -i src/lib/*.cpp
	apptainer exec $(SCRIPTPATH)/container/container.sif clang-format --style=Google -i src/lib/*.hpp
	apptainer exec $(SCRIPTPATH)/container/container.sif clang-format --style=Google -i src/tests/*.cpp

interfacingtask:
	# execute interfacing task with neural networks
	cd $(SCRIPTPATH)
	apptainer exec $(SCRIPTPATH)/container/container.sif python src/interfacing/application.py src/interfacing/approx.pdf
	

simulation:
	cd $(SCRIPTPATH)
	rm -rf ./simulation_results/mdsimulation/*.csv
	apptainer exec $(SCRIPTPATH)/container/container.sif ./out/simulations/mdsimulation ./simulation_results/mdsimulation

particlescaling:
	cd $(SCRIPTPATH)
	rm -rf ./simulation_results/particlecaling/*.csv
	apptainer exec $(SCRIPTPATH)/container/container.sif ./out/simulations/particlescaling ./simulation_results/particlescaling
	apptainer exec $(SCRIPTPATH)/container/container.sif python3 src/simulations/plot_particlescaling.py ./simulation_results/particlescaling ./simulation_results/particlescaling/scaling.pdf

ompstrongscaling:
	cd $(SCRIPTPATH)
	rm -rf ./simulation_results/strongscaling/*.csv
	apptainer exec $(SCRIPTPATH)/container/container.sif cmake -B out/ -S src/
	apptainer exec $(SCRIPTPATH)/container/container.sif cmake --build out/ --target strongscaling --parallel 
	echo "1 Thread"
	apptainer exec $(SCRIPTPATH)/container/container.sif ./out/simulations/strongscaling ./simulation_results/strongscaling 1
	echo "2 Threads"
	apptainer exec $(SCRIPTPATH)/container/container.sif ./out/simulations/strongscaling ./simulation_results/strongscaling 2
	echo "4 Threads"
	apptainer exec $(SCRIPTPATH)/container/container.sif ./out/simulations/strongscaling ./simulation_results/strongscaling 4
	echo "8 Threads"
	apptainer exec $(SCRIPTPATH)/container/container.sif ./out/simulations/strongscaling ./simulation_results/strongscaling 8
	echo "16 Threads"
	apptainer exec $(SCRIPTPATH)/container/container.sif ./out/simulations/strongscaling ./simulation_results/strongscaling 16
	echo "32 Threads"
	apptainer exec $(SCRIPTPATH)/container/container.sif ./out/simulations/strongscaling ./simulation_results/strongscaling 32
	echo "Plotting"
	apptainer exec $(SCRIPTPATH)/container/container.sif python3 src/simulations/plot_strongscaling.py ./simulation_results/strongscaling/runtimes.csv ./simulation_results/strongscaling/scaling.pdf

buildtarget:
	# to be used as:           make buildtarget ARGS="MDlib test_linked_cell"
	cd $(SCRIPTPATH)
	apptainer exec $(SCRIPTPATH)/container/container.sif cmake -B out/ -S src/
	apptainer exec $(SCRIPTPATH)/container/container.sif cmake --build out/ --target $(ARGS)

gprofrun:
	cd $(SCRIPTPATH)
	apptainer exec $(SCRIPTPATH)/container/container.sif cmake -DCMAKE_CXX_FLAGS=-pg -DCMAKE_EXE_LINKER_FLAGS=-pg -DCMAKE_SHARED_LINKER_FLAGS=-pg -B out/ -S src/
	apptainer exec $(SCRIPTPATH)/container/container.sif cmake --build out/ --target gprof_target
	apptainer exec $(SCRIPTPATH)/container/container.sif ./out/simulations/gprof_target
	apptainer exec $(SCRIPTPATH)/container/container.sif mv gmon.out ./simulation_results/profiling/
	apptainer exec $(SCRIPTPATH)/container/container.sif gprof -l ./out/simulations/gprof_target simulation_results/profiling/gmon.out > ./simulation_results/profiling/gprof_profile.txt

valgrindrun: build
	cd $(SCRIPTPATH)
	apptainer exec $(SCRIPTPATH)/container/container.sif valgrind --tool=cachegrind --cache-sim=yes --branch-sim=yes --cachegrind-out-file=./simulation_results/profiling/cachegrind.out out/simulations/valgrind_target
	apptainer exec $(SCRIPTPATH)/container/container.sif cg_annotate ./simulation_results/profiling/cachegrind.out > ./simulation_results/profiling/cachegrind_results.txt
