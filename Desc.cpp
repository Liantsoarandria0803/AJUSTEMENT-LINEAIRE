#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <string>
#include <cmath>

class LeastSquares {
private:
    std::vector<double> x_values;
    std::vector<double> y_values;
    std::vector<double> a_values;// slop list
    std::vector<double> b_values; // intercept list

    double a; // slope
    double b; // intercept

public:
    // Constructor
    LeastSquares() : a(0), b(0) {}

    // Method to read data from a file
     // Method to read data from a file
    void readData(const std::string& filename) {
        int size;
        double x,y;
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "ERREUR FILE FORMAT\n";
        }
        file >> size;
        for (int i=0;i<size;i++){
            file >> x;
            file >> y;
            x_values.push_back(x);
            y_values.push_back(y);
        }
        file.close();
    }

    // Method to perform the least squares fit using gradient descent
    void fit(double a0, double b0, int iterations, double lr) {
        if (x_values.size() != y_values.size()) {
            throw std::invalid_argument("x and y must have the same size");
        }

        a = a0;
        b = b0;

        int n = x_values.size();

        for (int iter = 0; iter < iterations; iter++) {
            double gradA = 0;
            double gradB = 0;

            // Calculate gradients
            for (int i = 0; i < n; i++) {
                double error = (a * x_values[i] + b) - y_values[i];
                gradA += error * x_values[i];
                gradB += error;
            }

            gradA /= n;
            gradB /= n;

            // Update coefficients
            a -= lr * gradA;
            a_values.push_back(a);
            b -= lr * gradB;
            b_values.push_back(b);
        }
        std::cout << "Loss :"<< LeastSquares::loss()<< std::endl;
    }

    // Method to predict the value of y for a given x
    double predict(double x) const {
        return a * x + b;
    }

    // Method to calculate the loss (Mean Squared Error)
    double loss() const {
        double total_loss = 0;
        int n = y_values.size();

        for (int i = 0; i < n; i++) {
            double error = y_values[i] - (a * x_values[i] + b);
            total_loss += error * error;
        }

        return total_loss / n;
    }

    // Method to print the results
    void printCoefficients() const {
        std::cout << "Slope (a): " << a << std::endl;
        std::cout << "Intercept (b): " << b << std::endl;
    }
      // Method to generate a gnuplot script and data file
    void generateGnuplotScript(const std::string& dataFilename, const std::string& scriptFilename) const {
        // Save data points to a file
        std::ofstream dataFile(dataFilename);
        for (size_t i = 0; i < x_values.size(); ++i) {
            dataFile << x_values[i] << " " << y_values[i] << "\n";
        }
        dataFile.close();

        // Create gnuplot script
         std::ofstream scriptFile(scriptFilename);
            scriptFile << "set title 'Comparison of all Lines'\n";
            scriptFile << "set xlabel 'X'\n";
            scriptFile << "set ylabel 'Y'\n";
            scriptFile << "set grid\n";
            scriptFile << "plot '" << dataFilename << "' using 1:2 with points title 'Data Points', \\\n";
            scriptFile << "     1 * x + 1 title 'First Line' with lines, \\\n";
            for(int i=2;i<a_values.size()-1;i++){
                if (i%500==0){
                    scriptFile << a_values[i]<< "* x +"<< b_values[i]<<" title '"<< i <<" Line' with lines, \\\n";
                }
            }
            scriptFile << "     10.0447 * x - 7.3948 title 'Fitted Line' with lines\n";
            scriptFile << "pause -1\n"; // Pause to keep the window open
            scriptFile.close();
    }
};

int main() {
    // Create an instance of LeastSquares
    LeastSquares ls;

    // Read data from file
    ls.readData("data.txt");

    // Perform the fit
    ls.fit(0.0, 0.0, 3500, 0.01);

    // Print the coefficients
    ls.printCoefficients();
    // Generate gnuplot script and data file
    ls.generateGnuplotScript("data.dat", "plot.txt");

    // Execute gnuplot
    system("gnuplot -persist plot.txt");

    // Predict a value
    double x_pred = 5;
    std::cout << "Predicted value for x = " << x_pred << " is y = " << ls.predict(x_pred) << std::endl;

    return 0;
}
