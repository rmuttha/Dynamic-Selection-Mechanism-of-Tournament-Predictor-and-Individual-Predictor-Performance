# Dynamic-Selection-Mechanism-of-Tournament-Predictor-and-Individual-Predictor-Performance
## 1. Project Overview
This repository contains the source code and documentation for the "Tournament Branch Predictor Study," a research project focused on improving processor performance by optimizing branch prediction algorithms. The project specifically investigates the dynamic selection capabilities of the Tournament Predictor compared to Bimodal, Gshare, and Piecewise predictors, utilizing simulations to analyze prediction accuracy and system efficiency.<br>

## 2. Background

  ### a. Bimodal Saturating Branch Predictor
The Bimodal Saturating Branch Predictor is a fundamental prediction scheme utilized in computer architecture. It operates based on a simple but effective finite state machine approach using Bimodal saturating counters. These counters monitor and predict branch behavior over time, adjusting their predictions based on the outcomes of previous branches. The counters typically encode four states: strongly taken, weakly taken, weakly not taken, and strongly not taken. This method allows the predictor to adapt to recent branch behaviors while maintaining stability against noise in prediction patterns.<br>

  ### b. Gshare Branch Predictor
The Gshare predictor improves forecast accuracy by combining aspects of local and global branch history. Introduced by Scott McFarling, this predictor uses a global history register (GHR) and an exclusive OR (XOR) operation to merge the branch address with the global history, creating an index for a prediction table. This technique helps to identify and exploit correlations between branch addresses and their historical outcomes, allowing the Gshare predictor to capture global patterns in branch behavior.<br>

  ### c. Piecewise Branch Predictor
This novel predictor addresses the varied and dynamic nature of program execution by segmenting it into different parts, each with its own prediction strategy. The Piecewise Branch Predictor (PBP) divides program execution into several segments based on specific parameters like program counters or instruction types. Each segment has its own set of prediction mechanisms, allowing for customized predictions that adapt to the unique behaviors of each segment. This strategy enhances accuracy by tailoring predictions to the specific dynamics of different program sections.<br>

  ### d. Tournament Branch Predictor
The Tournament Branch Predictor is an advanced prediction scheme that combines multiple predictors to leverage their collective strengths. It consists of several sub-predictors, such as the Bimodal and Gshare predictors, and includes a meta-predictor or selection mechanism that decides which sub-predictor to consult for each branch. This predictor dynamically selects the most accurate sub-predictor based on historical performance, effectively using group intelligence to maximize overall prediction accuracy. Its ability to dynamically adjust to varying program behaviors makes it highly effective in reducing mispredictions and enhancing processor throughput.<br>

## 3. Overall Concept
The effectiveness of branch prediction is a crucial factor in modern computer architectures, as accurate prediction of branch directions can significantly reduce pipeline stalls and increase computing throughput. Each of these predictors offers a different approach to forecasting branch results, and their performance can vary depending on the specific program behaviors and execution contexts. The study’s comparison of these predictors provides valuable insights into their relative strengths and weaknesses, informing future improvements in processor design.<br>

## 4. Repository Contents
### a. Branch_Predictors.cpp: Core implementation file containing all branch prediction algorithms and their evaluation logic.<br>
### b. Makefile: Build script for compiling the C++ code into an executable.<br>
### c. predictor: Executable file generated from the C++ source, ready to run the simulations.<br>
### d. Result.txt: Output file containing the results of the prediction simulations, detailing accuracy rates and other relevant data.<br>

## 5. Getting Started
### a. Prerequisites
Ensure you have a C compiler like GCC installed on your system to compile and run the simulation.<br>
### b. Building the Project
To compile the project, ensure you have a C++ compiler installed and configured (GCC recommended). Use the provided Makefile by running the following command in the terminal:<br><br>
`make` <br>
This command will compile Branch_Predictors.cpp and create the predictor executable.<br>

### c. Running the Simulations
To execute the branch predictors, use the compiled executable. You can run the program directly if you are on a Unix-like system:<br><br>
`./predictor` <br>
This will process the input data and generate output in Result.txt detailing the performance of each predictor.<br>

## 6. Research Outcomes
The project analyzes the efficiency and accuracy of different branch prediction algorithms, highlighting how the Tournament Predictor dynamically selects the most accurate predictor based on historical performance. Key findings include the adaptability of the Tournament Predictor in various program scenarios, leading to reduced mispredictions and enhanced processing efficiency.<br>

## 7. Contributing
Contributions to this project are welcome. Please fork the repository, create a feature branch, and submit a pull request.<br>

## 8. Acknowledgments
Professor Dr. Yuchen Huang for guidance and mentorship.<br>
Teaching Assistant Venkatesh Srinivas for support and valuable feedback.<br>

## 9. Contact
For any further queries regarding this project, please contact rmuttha@pdx.edu<br>

## 10. References
Detailed references and literature reviewed can be found in the accompanying project report.
