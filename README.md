# Epidemic Spread Simulation Project in C++

### Project Explanation
The Epidemic Spread Simulation Project, firstly done created in <a href="https://github.com/QuiteNew/Computing-and-Algorithms-Project-"><u>Flowgorithm</u></a> for the subject Computing and Algorithms can be expanded to a more complex project, including features such as complex epidemic spread models, interaction between people, and more user interaction. 
This project is for the subject Objective Programming

### Project Expansion Details
The epidemic spread simulation project will simulate the spread of a disease (virus) in a population over time. The disease will impact the population in different ways, depending on the rate of transmission, rate of recovery, and interaction between people. The simulation will include time-stepping to simulate the spread of the disease over multiple cycles (days or weeks). Users will be able to change parameters such as the infection rate, number of people initially infected, and other parameters. Core Features: Population Structure: The population will be made up of people, each of whom will have a status: Susceptible (S), Infected (I), or Recovered (R). Each person can change status from S to I (infection), and from I to R (recovery). The simulation will track these changes over time.

More details about this project can be found in the project documentation. 

### Model of the simulation
Transmission & Recovery Model: Infection: Susceptible individuals become infected based on the infection rate (β). This depends on interactions with infected individuals. Recovery: Infected individuals recover over time based on a recovery rate (γ), moving from Infected (I) to Recovered (R) after a certain period. 
Interaction and Randomness: Simulate random interactions between individuals, where infected people can spread the disease to nearby susceptible individuals. Randomness can be implemented using random number generators to simulate the probability of infection transmission. 
Time-Stepping Simulation: The disease spreads and individuals recover over time. Each "time step" represents one day or one week of the simulation. You can choose how many days to simulate, and at each step, the population’s states are updated. 
Statistics Tracking: Track statistics such as: Total number of infected individuals over time. Total number of recovered individuals. Total number of susceptible individuals. Peak number of infected individuals during the simulation. 
End-point: when the epidemic is over, and the disease is no longer spreading (i.e., all infected individuals have recovered or there are no more susceptible individuals left). User Input & Controls: Users can set parameters like: Initial infected population. Infection rate (β). Recovery rate (γ). Total population. Number of days to simulate. Maximum interactions per day (how many times people can interact with others). Allow users to modify these parameters during the simulation to see how different factors influence the spread of the disease. 

### Algorithmic Breakdown
The iterative algorithm is employed in the visual simulation of the epidemic’s impact on the population over a number of days; each day is equivalent to one iteration of the simulation of the spread of the disease (0 to maximum, currently set at 30 days).
The primary method of the simulation of the spread of the disease by the EpidemicSimulation class is the utilization of one while loop that cycles through each day (one iteration) until the user closes the window or reaches the maximum number of days (30).
Within each iteration of the while loop, one day of simulation is simulated by considering any events that occurred during the previous day, engaging in the simulation steps (such as vaccinating, re-infecting, infecting, recovering, and/or killing people), updating the movements of people, creating graphic representations of what has occurred, and updating simulation statistics (in other words, the number of people in each of the possible states: infected, immune, dead) on an active basis. Each day of the while loop simulation is O(N) linear time complexity because it involves a large number of for-loops that cycle through the entire population vector for all state changes or graphic changes that occur.

### Simulation Workflow
In this epidemic simulation, the workflow models the progression of a disease through a population over a series of days. The program first creates a population of Person objects that have randomly generated ages and vaccination statuses. Additionally, a certain number of the Person objects are instantiated as already infected with disease (based on the starting conditions). 
Each day is made up of three phases: 
1. Checking for mutation of the virus in approximately 10% of the total number of days.
2. Processing all of the currently infected individuals for new cases, recoveries, or death before starting new infections of susceptible individuals.
3. Updating vaccination coverage and tracking milestones.

At the end of each day, using the simulation statistics (the number of susceptible, infected, recovered and dead), a progress bar will be displayed and continue until all individuals are no longer susceptible/infected or the maximum number of days (or both) has occurred. After completion of the simulation, a detailed summary report will be created showing the overall statistics, details of the virus mutation (if enabled), vaccination progress, age distribution of the population and possibly an interactive search through the population. Overall, this iterative, structured approach accurately models the core dynamics of an epidemic while being easily extendable due to its modular design.

### Important Notes
Taken from project assignment:
Keep in mind the complexity of the project assignment. Because you are working a maximum of 3 people per project, all of the members need to complete at least 1 larger assignment in the group.
- The project needs to include the basic concepts that act as the building blocks of Object-oriented Programming (OP): Encapsulation, Abstraction, Polymorphism, Inheritance, per group - decide among the members which student will work on what.
- The entire project assignment should be ONE whole project, meaning all of the work of each of the group members needs to be combined into one piece.
- All members of the group should contribute to the completion of the project.
- All members of the group should give an oral presentation of their working section of the project during the last week of classes (i.e., May 12/15, 2025).


