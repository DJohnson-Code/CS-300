//============================================================================
// Author      : Darnell Johnson
// Course      : CS 300 - Data Structures and Algorithms
// Professor   : Professor Susalla
//============================================================================


#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

struct Course {
    std::string courseNumber;
    std::string courseName;
    std::vector<std::string> prerequisites;
};

struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(const Course& newCourse) : course(newCourse), left(nullptr), right(nullptr) {}
};

class BinarySearchTree {
private:
    Node* root = nullptr;

    void addNode(Node* node, const Course& course) {
        // Smaller course numbers go left
        if (course.courseNumber < node->course.courseNumber) {
            if (node->left == nullptr) {
                node->left = new Node(course);
            } else {
                addNode(node->left, course);
            }
        } else {
            // Equal or larger course numbers go right
            if (node->right == nullptr) {
                node->right = new Node(course);
            } else {
                addNode(node->right, course);
            }
        }
    }

    // In-order traversal prints the courses sorted by course number
    void printInOrder(Node* node) const {
        if (node == nullptr) {
            return;
        }

        // Left subtree first, then this node, then the right subtree
        printInOrder(node->left);
        std::cout << node->course.courseNumber << ", " << node->course.courseName << '\n';
        printInOrder(node->right);
    }

    // Post-order delete so the children are freed before their parent
    void deleteNodes(Node* node) {
        if (node == nullptr) {
            return;
        }

        deleteNodes(node->left);
        deleteNodes(node->right);
        delete node;
    }

public:
    // Destructor cleans up all nodes so no memory leaks
    ~BinarySearchTree() {
        clear();
    }

    void clear() {
        deleteNodes(root);
        root = nullptr;
    }

    void insert(const Course& course) {
        if (root == nullptr) {
            root = new Node(course);
        } else {
            addNode(root, course);
        }
    }

    const Course* findCourse(const std::string& courseNumber) const {
        Node* current = root;

        // Search down the tree, going left or right based on the comparison
        while (current != nullptr) {
            if (courseNumber == current->course.courseNumber) {
                return &current->course;
            } else if (courseNumber < current->course.courseNumber) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        return nullptr;
    }

    void printCourseList() const {
        printInOrder(root);
    }
};

std::string trim(const std::string& text) {
    size_t first = text.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return "";
    }

    size_t last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

std::string upperCase(std::string text) {
    for (char& letter : text) {
        letter = static_cast<char>(std::toupper(static_cast<unsigned char>(letter)));
    }
    return text;
}

// Reads the CSV file and fills validCourses with only the courses that pass validation
bool parseCourseFile(const std::string& fileName, std::vector<Course>& validCourses) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cout << "Unable to open " << fileName << ".\n";
        return false;
    }

    std::vector<Course> parsedCourses;
    std::vector<std::string> courseNumbers;
    std::queue<std::string> invalidCourses;
    std::string line;

    while (std::getline(file, line)) {
        if (trim(line).empty()) {
            continue;
        }

        // Split the line on commas into separate fields
        std::vector<std::string> fields;
        std::stringstream row(line);
        std::string field;

        while (std::getline(row, field, ',')) {
            fields.push_back(trim(field));
        }

        // Ignore the empty prerequisite fields
        while (fields.size() > 2 && fields.back().empty()) {
            fields.pop_back();
        }

        // A course needs at least a number and a name with no blank fields
        if (fields.size() < 2 ||
            std::find(fields.begin(), fields.end(), "") != fields.end()) {
            std::cout << "Skipping a line with missing course information.\n";
            continue;
        }

        // Only keep the first copy of a course number
        std::string number = upperCase(fields[0]);
        if (std::find(courseNumbers.begin(), courseNumbers.end(), number) != courseNumbers.end()) {
            std::cout << "Skipping duplicate course: " << number << '\n';
            continue;
        }

        Course course;
        course.courseNumber = number;
        course.courseName = fields[1];

        for (size_t i = 2; i < fields.size(); ++i) {
            course.prerequisites.push_back(upperCase(fields[i]));
        }

        parsedCourses.push_back(course);
        courseNumbers.push_back(number);
    }

    // Check prerequisites after reading the entire file
    std::vector<bool> valid(parsedCourses.size(), true);
    for (size_t i = 0; i < parsedCourses.size(); ++i) {
        for (const std::string& prerequisite : parsedCourses[i].prerequisites) {
            // Reject the course if a prerequisite isn't in the file
            if (std::find(courseNumbers.begin(), courseNumbers.end(), prerequisite) == courseNumbers.end()) {
                std::cout << "Skipping " << parsedCourses[i].courseNumber
                          << ": prerequisite " << prerequisite << " was not found.\n";
                valid[i] = false;
                invalidCourses.push(parsedCourses[i].courseNumber);
                break;
            }
        }
    }

    // Skip courses that depend on a rejected course
    while (!invalidCourses.empty()) {
        std::string rejectedNumber = invalidCourses.front();
        invalidCourses.pop();

        for (size_t i = 0; i < parsedCourses.size(); ++i) {
            if (valid[i] &&
                std::find(parsedCourses[i].prerequisites.begin(),
                          parsedCourses[i].prerequisites.end(), rejectedNumber)
                    != parsedCourses[i].prerequisites.end()) {
                std::cout << "Skipping " << parsedCourses[i].courseNumber
                          << ": prerequisite " << rejectedNumber << " was rejected.\n";
                valid[i] = false;
                invalidCourses.push(parsedCourses[i].courseNumber);
            }
        }
    }

    validCourses.clear();
    for (size_t i = 0; i < parsedCourses.size(); ++i) {
        if (valid[i]) {
            validCourses.push_back(parsedCourses[i]);
        }
    }

    return true;
}

// Parses the file and loads the valid courses into the tree
bool loadBST(const std::string& fileName, BinarySearchTree& courses) {
    std::vector<Course> validCourses;
    if (!parseCourseFile(fileName, validCourses)) {
        return false;
    }

    if (validCourses.empty()) {
        std::cout << "No valid courses found in the file.\n";
        return false;
    }

    // Clear old data first so loading a second file doesn't mix courses
    courses.clear();
    for (const Course& course : validCourses) {
        courses.insert(course);
    }

    return true;
}

// Looks up one course and prints its info and prerequisites
void searchCourse(const BinarySearchTree& courses, const std::string& courseNumber) {
    const Course* course = courses.findCourse(upperCase(trim(courseNumber)));
    if (course == nullptr) {
        std::cout << "Course not found.\n";
        return;
    }

    std::cout << course->courseNumber << ", " << course->courseName << '\n';

    if (course->prerequisites.empty()) {
        std::cout << "Prerequisites: None\n";
        return;
    }

    // Look up each prerequisite to print its full name too
    std::cout << "Prerequisites:\n";
    for (const std::string& number : course->prerequisites) {
        const Course* prerequisite = courses.findCourse(number);
        std::cout << prerequisite->courseNumber << ", " << prerequisite->courseName << '\n';
    }
}

int main() {
    BinarySearchTree courses;
    bool loaded = false;
    std::string choice;

    std::cout << "Welcome to the course planner.\n";

    // Menu loop keeps running until the user picks 9 or input ends
    while (true) {
        std::cout << "\n    1. Load Data Structure.\n"
                  << "    2. Print Course List.\n"
                  << "    3. Print Course.\n"
                  << "    9. Exit\n\n"
                  << "What would you like to do? ";

        if (!std::getline(std::cin, choice)) {
            break;
        }
        choice = trim(choice);

        if (choice == "1") {
            std::string fileName;
            std::cout << "Enter the course file name: ";
            std::getline(std::cin, fileName);

            if (loadBST(trim(fileName), courses)) {
                loaded = true;
                std::cout << "Courses loaded successfully.\n";
            } else {
                std::cout << "Unable to load courses.\n";
            }
        } else if (choice == "2") {
            // Data has to be loaded before printing or searching
            if (!loaded) {
                std::cout << "Please load courses first.\n";
            } else {
                std::cout << "Here is a sample schedule:\n\n";
                courses.printCourseList();
            }
        } else if (choice == "3") {
            if (!loaded) {
                std::cout << "Please load courses first.\n";
            } else {
                std::string courseNumber;
                std::cout << "What course do you want to know about? ";
                std::getline(std::cin, courseNumber);
                searchCourse(courses, courseNumber);
            }
        } else if (choice == "9") {
            std::cout << "Thank you for using the course planner!\n";
            break;
        } else {
            std::cout << choice << " is not a valid option.\n";
        }
    }

    return 0;
}

