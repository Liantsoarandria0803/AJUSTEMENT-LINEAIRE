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
    double a; // slope
    double b; // intercept

public:
    // Constructor
    LeastSquares() : a(0), b(0) {}

    // Method to read data from a file
    void readData(const std::string& filename) {
        int size;
        double x,y;
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "ERREUR FILE FORMAT\n";
        }
        file >> size;
        for (int i=0;i<size*2;i++){
            file >> x;
            file >> y;
            x_values.push_back(x);
            y_values.push_back(y);
        }
        file.close();
    }

    // Method to perform the least squares fit
    void fit() {
        if (x_values.size() != y_values.size()) {
            throw std::invalid_argument("x and y must have the same size");
        }

        double x_sum = 0, y_sum = 0, xy_sum = 0, x2_sum = 0;
        int n = x_values.size();

        for (int i = 0; i < n; i++) {
            x_sum += x_values[i];
            y_sum += y_values[i];
            xy_sum += x_values[i] * y_values[i];
            x2_sum += x_values[i] * x_values[i];
        }

        double denominator = (n * x2_sum - x_sum * x_sum);
        if (denominator == 0) {
            throw std::runtime_error("Denominator is zero, can't perform fit");
        }

        a = (n * xy_sum - x_sum * y_sum) / denominator;
        b = (y_sum * x2_sum - x_sum * xy_sum) / denominator;
    }

    // Method to predict the value of y for a given x
    double predict(double x) const {
        return a * x + b;
    }

    // Method to print the results
    void printCoefficients() const {
        std::cout << "Slope (a): " << a << std::endl;
        std::cout << "Intercept (b): " << b << std::endl;
    }
      double loss() const {
        double total_loss = 0;
        int n = y_values.size();

        for (int i = 0; i < n; ++i) {
            double error = y_values[i] - (a * x_values[i] + b);
            total_loss += error * error;
        }

        return total_loss / n;
    }
     void generateGnuplotScript(const std::string& dataFilename, const std::string& scriptFilename) const {
        // Save data points to a file
        std::ofstream dataFile(dataFilename);
        for (size_t i = 0; i < x_values.size(); ++i) {
            dataFile << x_values[i] << " " << y_values[i] << "\n";
        }
        dataFile.close();

        // Create gnuplot script
        std::ofstream scriptFile(scriptFilename);
        scriptFile << "set title 'Least Squares Regression'\n";
        scriptFile << "set xlabel 'x'\n";
        scriptFile << "set ylabel 'y'\n";
        scriptFile << "plot '" << dataFilename << "' using 1:2 with points title 'Data Points', ";
        scriptFile << a << " * x + " << b << " title 'Fitted Line' with lines\n";
        scriptFile << "pause -1\n";
        scriptFile.close();
    }
};

int main() {
    // Create an instance of LeastSquares
    LeastSquares ls;

        // Read data from file
        ls.readData("data.txt");
        ls.generateGnuplotScript("data.dat", "plot.gp");

        // Perform the fit
        ls.fit();

        // Print the coefficients
        ls.printCoefficients();
         // Generate gnuplot script and data file
        ls.generateGnuplotScript("data.dat", "plot.gp");

    // Execute gnuplot
    system("gnuplot -persist plot.gp");

        // Predict a value
        double x_pred = 5;
        std::cout << ls.loss()<< std::endl;

        std::cout << "Predicted value for x = " << x_pred << " is y = " << ls.predict(x_pred) << std::endl;
    return 0;
}
