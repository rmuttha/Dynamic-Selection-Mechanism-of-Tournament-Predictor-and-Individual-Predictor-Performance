compile: Branch_Predictors.cpp 
	g++ Branch_Predictors.cpp -o predictor

run: compile
	./predictor Trace.txt

clean:
	rm -rf predictor
	rm -rf Result.txt
