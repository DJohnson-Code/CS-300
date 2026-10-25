# CS 300 - Data Structures and Algorithms

This repository contains my coursework for CS 300 at Southern New Hampshire University. The two main projects involved designing and building a course planning program that allows academic advisors to load course information, search for specific courses, view prerequisites, and print a list of courses in alphanumeric order.

## Portfolio Artifacts

- [Project One: Pseudocode and Runtime Analysis](Module-6/ProjectOne.docx)
- [Project Two: Course Planner in C++](Module-7/ProjectTwo.cpp)

## Project Reflection

### What was the problem you were solving in the projects for this course?

The goal of these projects was to develop a program that could organize and display course information for academic advisors. In Project One I focused on planning the program and comparing different data structures to figure out which would work best. Project Two involved taking that design and actually implementing the program in C++

### How did you approach the problem? Consider why data structures are important to understand.

I started by breaking down the requirements and looking at how a vector, hash table, and binary search tree would handle them. I compared their runtime and memory usage, along with how each would handle searching and sorting. I ended up choosing a binary search tree because its in order traversal would let me print the courses alphabetically without needing a separate sorting step. This helped me better understand why choosing the right data structure matters before getting into the actual code.

### How did you overcome any roadblocks you encountered while going through the activities or project?

One of the challenges was figuring out how to handle invalid course information. Particularly missing prerequisites and courses that depended on them. I also had to work through how the binary search tree would insert, search for, and print courses correctly. Going back through my pseudocode and breaking things down into smaller steps helped quite a bit. I also tested the different menu options and course searches to make sure everything worked as expected.

### How has your work on this project expanded your approach to designing software and developing programs?

I think the biggest takeaway for me was reaffirming the importance of planning before jumping into coding. It's easy to focus on just getting something to work but this course helped reaffirm that the way you structure a solution matters too. Two different data structures might accomplish the same thing but that doesn't necessarily mean they're equally suited for the problem. I'll definitely be keeping that in mind with future projects.

### How has your work on this project evolved the way you write programs that are maintainable, readable, and adaptable?

I've gotten better about separating different parts of a program into their own functions rather than trying to handle everything in one place. For example the course planner has separate functions for reading the CSV file, loading the tree, searching for courses, and displaying information. That makes the program easier to follow and troubleshoot. I've also been paying more attention to naming, comments, and keeping the code understandable so that it's easier to come back to later or make changes to.
