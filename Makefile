all:
	nvcc -std=c++11 main.cu Hierarchical_clustering.cpp FileOperator.cpp -o changeDetection
clean:
	rm -rf *o changeDetection
