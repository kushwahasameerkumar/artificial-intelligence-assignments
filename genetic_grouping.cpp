#include<bits/stdc++.h>

#define POPULATION_SIZE 6

using namespace std;

struct Chromosome{
    vector<vector<int>> groups;     //starting index for group number is 0
    vector<int> genes;              //store group number of r-th student
    int fitness_score;              //store fitness-value
};

typedef struct Chromosome Chromosome;

struct Population{
    vector<Chromosome> chromosomes;
};

typedef struct Population Population;


//Variables declaration

Population population;              //to store population
vector<int> marks;                  //input marks for a student -> sum -> insert into marks vector

int no_of_students, no_of_subjects, no_of_groups;

int fitness_count = 0;
int lowestTotalFitness = INT_MAX;
Chromosome fittest;

void input(){
    cout << "Enter number of students: ";
    cin >> no_of_students;
    cout << "Enter number of subjects: ";
    cin >> no_of_subjects;
    cout << "Enter number of groups: ";
    cin >> no_of_groups;
    cout << endl;
    
    for (int i = 0; i < no_of_students; ++i)
    {
        cout << "Enter marks of student " << i + 1 << ": ";
        int temp_marks;
        int temp_total = 0;
        for (int j = 0; j < no_of_subjects; ++j)
        {
            cin >> temp_marks;
            temp_total += temp_marks;
        }
        int avg = temp_total / no_of_subjects;
        marks.push_back(avg);
    }
}

int randomGroup(){
    //min is 0
    return rand() % no_of_groups;
}

int square(int n){
    return n * n;
}

void calculateFitness()
{
    cout << "\n=>Calculate fitness\n";
    for (int i = 0; i < population.chromosomes.size(); ++i)
    {
        Chromosome *chromosome = &(population.chromosomes[i]);
        chromosome->fitness_score = 0;

        for (int j = 0; j < chromosome->groups.size(); ++j)
        {
            if(chromosome->groups[j].size() == 0){
                //no student in the group
                chromosome->fitness_score = INT_MAX;
                break;
            }

            int avg = 0;
            int fitness = 0;

            for (auto k : chromosome->groups[j])
            {
                avg += k;
            }
            avg = avg/chromosome->groups[j].size();

            for(auto k: chromosome->groups[j])
            {
                fitness += square(k - avg);
            }

            // fitness /= chromosome->groups[j].size();

            chromosome->fitness_score += fitness;
        }
    }
}

int indexAlreadyPresent(int index, vector<int> indices){
    for (int i = 0; i < indices.size(); ++i)
    {
        if(indices[i] == index){
            return i;
        }
    }
    return INT_MIN;
}

void initializePopulation(){
    cout << "\n=>Initialize popultaion\n";
    //initialize with 6 populations
    population.chromosomes.resize(POPULATION_SIZE);

    for (int i = 0; i < population.chromosomes.size(); ++i)
    {
        Chromosome *chromosome = &(population.chromosomes[i]);
        chromosome->groups.resize(no_of_groups);

        vector<int> groupIndices;
        for (int k = 0; k < no_of_groups; ++k)
        {
            int index = rand() % no_of_students;
            while (indexAlreadyPresent(index, groupIndices) >= 0)
            {
                index = rand() % no_of_students;
            }
            groupIndices.push_back(index);
        }

        for (int j = 0; j < no_of_students; ++j)
        {
            int groupNumber = indexAlreadyPresent(j, groupIndices) >=0 ? indexAlreadyPresent(j, groupIndices) : randomGroup();
            chromosome->groups[groupNumber].push_back(marks[j]);
            chromosome->genes.push_back(groupNumber);
        }
    }
}

bool sortByFitnessScore(Chromosome A, Chromosome B){
    return A.fitness_score < B.fitness_score;
}

void selection(){
    cout << "\n=>Selection\n";
    sort(population.chromosomes.begin(), population.chromosomes.end(), sortByFitnessScore);         //sort in ascending order of fitness score
    population.chromosomes.resize(POPULATION_SIZE);                                                 //resize to contain 6 populations only, discard others with hiher fitness score i.e. higher diversity
}

int randomCrossOverPoint(){
    return 1 + (rand() % (no_of_students - 1));
}

void crossOver(){
    cout << "\n=>Crossover\n";
    //i, j represents first and second parent respectively
    int population_size = population.chromosomes.size();
    for (int i = 0, j = 1; j < population_size; i += 2, j += 2)
    {
        Chromosome parentA = population.chromosomes[i];
        Chromosome parentB = population.chromosomes[j];

        Chromosome offspringA, offspringB;
        offspringA.groups.resize(no_of_groups);
        offspringB.groups.resize(no_of_groups);

        int crossOverPoint = randomCrossOverPoint();

        for (int k = 0; k < no_of_students; ++k)
        {
            if(k < crossOverPoint){
                int groupNumber;

                groupNumber = parentB.genes[k];                     //for offspring A
                offspringA.genes.push_back(groupNumber);
                offspringA.groups[groupNumber].push_back(marks[k]);

                groupNumber = parentA.genes[k];                     //for offspring B
                offspringB.genes.push_back(groupNumber);
                offspringB.groups[groupNumber].push_back(marks[k]);

            }else{
                int groupNumber;

                groupNumber = parentA.genes[k];                     //for offspring A
                offspringA.genes.push_back(groupNumber);
                offspringA.groups[groupNumber].push_back(marks[k]);

                groupNumber = parentB.genes[k];                     //for offspring B
                offspringB.genes.push_back(groupNumber);
                offspringB.groups[groupNumber].push_back(marks[k]);
            }
        }

        population.chromosomes.push_back(offspringA);               //push offspring A in population
        population.chromosomes.push_back(offspringB);               //push offspring B in population
    }
}

bool mutationBoolean(){
    int mutation_probability = (rand() % 10) / 7;
    return mutation_probability > 0 ? true : false;
}

void mutation(){
    cout << "\n=>Mutation\n";
    for (int k = POPULATION_SIZE; k < population.chromosomes.size(); ++k)
    {
        Chromosome *chromosome = &(population.chromosomes[k]);

        for (int i = 0; i < no_of_students; ++i)
        {
            if(mutationBoolean){
                int swapIndex = rand() % no_of_students;
                int indexOfFirstGeneInGroup = 0;
                int indexOfSecondGeneInGroup = 0;
                int firstGroup = chromosome->genes[i];
                int secondGroup = chromosome->genes[swapIndex];

                for (int j = 0; j < i; ++j)
                {
                    if(chromosome->genes[j] == firstGroup){
                        indexOfFirstGeneInGroup++;
                    }
                }

                for (int j = 0; j < swapIndex; ++j)
                {
                    if (chromosome->genes[j] == secondGroup)
                    {
                        indexOfSecondGeneInGroup++;
                    }
                }

                //mutate
                chromosome->genes[i] = secondGroup;
                chromosome->genes[swapIndex] = firstGroup;

                int tempMarks = chromosome->groups[firstGroup][indexOfFirstGeneInGroup];
                chromosome->groups[firstGroup][indexOfFirstGeneInGroup] = chromosome->groups[secondGroup][indexOfSecondGeneInGroup];
                chromosome->groups[secondGroup][indexOfSecondGeneInGroup] = tempMarks;
            }
        }
    }
}

bool populationConverged(){
    int totalFitness = 0;
    bool r = false;
    for (auto chromosome : population.chromosomes)
    {
        totalFitness += chromosome.fitness_score;
    }
    
    if(totalFitness < lowestTotalFitness){
        lowestTotalFitness = totalFitness;
        fitness_count = 0;
    }else{
        fitness_count++;
    }

    if(fitness_count >= 20){
        r = true;           //population converges if last 20 generations didn't had any increment in totalFitness value.
    }

    return r;
}

void displayPopulation(){
    cout << "\n------------------------Current populations------------------------\n\n";
    cout << "* A higher fitness score signifies higher diversity in distribution and vice-versa.\n";
    // cout << "* Parents and offsprings of the genereation are being shown only.\n\n";
    cout << "* Fittest 6 (i.e selecions / default POPULATION_SIZE) from previous generation are being shown only.\n";
    cout << "* Performance can be made better by increasing the limit of fitness_count variable i.e. controlling convergence. Currently set to 20 iteration if no update found in population, set it to 1000+ for large number of students for better performance.\n\n";
    for (int i = 0; i < population.chromosomes.size(); ++i)
    {
        cout << "chromosome " << i + 1 << " :\t";
        for (auto j : population.chromosomes[i].genes)
        {
            cout << j + 1 << " ";
        }
        cout <<"\tfitness score = "<<population.chromosomes[i].fitness_score<< endl;
    }
}

int main(){
    //setting for rand()
    srand((unsigned)time(0));

    //input required parameters
    input();

    initializePopulation();
    calculateFitness();
    displayPopulation();

    while (!populationConverged())
    {
        selection();
        displayPopulation();
        crossOver();
        mutation();
        calculateFitness();
        // displayPopulation();
    }

    selection();
    displayPopulation();

    cout << "\n=> Population converged\n";

    cout << "\n\n\n=>=> Solution: Chromosome with the least fitness score in the last population is a better distribution with less diversity of marks.\n\n";
    cout << "* As the initial population was totaly a random distribution and crossovers depends on it to a huge extent, so performance also varies with different executions of the program.\n";
    return 0;
}
