build:
	javac ./*.java

runjava: build
	java Simulation 300 50 0.2 5.0 > output.java.txt

runpython:
	python3 -m main > output.py.txt

clean:
	rm -f ./*.class
