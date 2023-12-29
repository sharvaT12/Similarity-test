//  driver.h
//
//  This file has 2 structs defined, they being Question and Person.
//  This file has all the function declaration to process the answers
//  from the user and find the most similar person in set of person.
//


#include <math.h>

#include <limits>

#include <string>

#include <map>

#include <set>

#include "myrandom.h"

using namespace std;

constexpr double lowest_double = std::numeric_limits < double > ::lowest();

/* Type: Question
 *
 * Type representing a personality quiz question.
 */
struct Question {
    string questionText;  //  Text of the question
    map < char, int > factors;  //  Map from factors to +1 or -1
    friend bool operator < (const Question & lhs,
        const Question & rhs) {
        return lhs.questionText < rhs.questionText;
    }
    friend bool operator == (const Question & lhs,
        const Question & rhs) {
        return lhs.questionText == rhs.questionText;
    }
    friend bool operator != (const Question & lhs,
        const Question & rhs) {
        return lhs.questionText != rhs.questionText;
    }
};

/* Type: Person
 *
 * Type representing a person, used to represent people when determining
 * who's the closest match to the user.
 */
struct Person {
    string name;  // Name of the person
    map < char, int > scores;  // Map from factors to +1 or -1
    friend bool operator < (const Person & lhs,
        const Person & rhs) {
        return lhs.name < rhs.name;
    }
    friend bool operator == (const Person & lhs,
        const Person & rhs) {
        return lhs.name == rhs.name;
    }
    friend bool operator != (const Person & lhs,
        const Person & rhs) {
        return lhs.name != rhs.name;
    }
};

/* randomElement
 *
 * This function selects, at random, a Question from the inputted questions set
 * and returns the question.  Note, this function does not remove the randomly
 * selected question from the set.
 */
Question randomElement(set < Question > & questions) {
    int ind = randomInteger(0, (int) questions.size() - 1);
    int i = 0;
    for (auto e : questions) {
        if (i == ind) {
            return e;
        }
        i++;
    }
    return {};
}

//
//  randomQuestionFrom
//
//  This function takes input as set of Question is passed by reference.
//
//  Then calls the randomElement function which returns a random question from
//  the set of questions, then deletes and returns the question.
//
Question randomQuestionFrom(set < Question > & questions) {
    if (questions.size() == 0) {
        throw runtime_error("Set: empty");
    }

    auto temp = randomElement(questions);
    questions.erase(temp);

    return temp;
}

//
//  scoresFrom
//
//  This function takes input as map<Question,int> passed by reference.
//
//  This function then loops through the map and takes the user's answer
//  from the map and processes it and stores into a map<char,int>
//  where char is one of the characters from "OCEAN" and int is the
//  value.
//  This function returns map<char, int>.
//
map < char, int > scoresFrom(map < Question, int > & answers) {
    map < char, int > S;

    for (const pair < Question, int > & A : answers) {
        int num = 0;

        if (A.second == 5) {
            num = 2;
        } else if (A.second == 4) {
            num = 1;
        } else if (A.second == 3) {
            num = 0;
        } else if (A.second == 2) {
            num = -1;
        } else if (A.second == 1) {
            num = -2;
        }

        for (const pair < char, int > & C : A.first.factors) {
            S[C.first] = S[C.first] + (C.second * num);
        }
    }
    return S;
}

//
//  normalize
//
//  This function takes input as map<char,int> which is passed by reference.
//
//  This function adds all the square of integers in the set that is int^2 and
//  stores it in a double, then gets the square root of the double and
//  divdes all the integers in the map<char,int> by the square root of
//  the total and stores it into map<char,double> and returns it.
//
map < char, double > normalize(map < char, int > & scores) {
    map < char, double > S;
    double total = 0;

    for (const pair < char, int > & A : scores) {
        total += pow(A.second, 2);
    }

    if (total == 0) {
        throw runtime_error("Map: has no nonzero value");
    }

    double normalize = sqrt(total);

    for (const pair < char, int > & Z : scores) {
        S[Z.first] = (Z.second / normalize);
    }
    return S;
}

//
//  cosineSimilarityOf
//
//  This function takes input as two map<char,double> which are passed 
//  by refrence
//
//  Here one map is Normalized answers from user and other is Normalized people
//  from the people file.
//
//  This function has a search algorith with time complexity O(N) if the char of
//  the two maps match then the two int are multiplied and added to a double and
//  next element is accessed. If the char don't match then the char which is
//  smaller is moved forward.
//
double cosineSimilarityOf(const map < char, double > & lhs,
    const map < char, double > & rhs) {

    double similar = 0;

    auto itr1 = lhs.begin();
    auto itr2 = rhs.begin();

    while (itr1 != lhs.end() && itr2 != rhs.end()) {
        if (itr1 -> first == itr2 -> first) {
            double mult = itr1 -> second * itr2 -> second;
            similar += mult;
            itr1++;
            itr2++;
        } else if (itr1 -> first > itr2 -> first) {
            itr2++;
        } else {
            itr1++;
        }
    }

    return similar;
}

//
//  mostSimilarTo
//
//  This function has two parameters one being map<char,int> and other
//  set<Person> which are passed by reference.
//
//  This function first calls normalize function on the two map<char,int> and
//  stores it into two map<char,double>. Then assisgns the double lowest
//  with the double returned cosineSimilarityOf of map<char, int> and first
//  person in set<Person>. Then this loops through set<Person> and
//  then calls cosineSimilarityOf for every person in the set and find
//  the person with most similarity and returns the person.
//
Person mostSimilarTo(map < char, int > & scores, set < Person > & people) {
    //  if the set people is empty.
    if (people.size() == 0) {
        throw runtime_error("People: empty");
    }
    Person Similar_per;
    Similar_per = * people.begin();
    map < char, double > Normalized = normalize(scores);

    map < char, double > per_normalized = normalize(Similar_per.scores);

    //  assigning lowest with similarity
    double lowest = cosineSimilarityOf(Normalized, per_normalized);

    double similarity = 0;

    for (auto A : people) {
        per_normalized = normalize(A.scores);
        similarity = cosineSimilarityOf(Normalized, per_normalized);

        if (similarity > lowest) {  //  if the similarity is greater
            lowest = similarity;
            Similar_per = A;
        }
    }
    return Similar_per;
}
