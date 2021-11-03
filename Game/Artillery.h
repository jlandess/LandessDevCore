namespace AI {
    namespace Artillery
    {
#include <iostream>
#include <ctime>
#include <math.h>
#include <cstdlib>

        using namespace std;

        const int kNumShells = 10;  // allowed 10 shells per target
        const int kMinDist = 200;  // min distance for a target
        const int kMaxDist = 900;  // max distance for a target
        const double kVelocity = 200.0;  // initial velocity of 200 ft/sec
        const double kGravity = 32.2;  // gravity for distance calculation
        const double kPi = 3.1415;

        // Returns the distance a shot travels given its angle.
        int DistanceCalc (double in_angle) {

            double time_in_air;
            // The following calculates how far the shot goes given
            // its angle of projection, velocity, and how long it stays
            // in the air.
            time_in_air = (2.0 * kVelocity * sin(in_angle)) / kGravity;
            return (int) round((kVelocity * cos(in_angle)) * time_in_air);
        }

        // Get user's angle input and calculates distance where shot lands.
        // Returns the distance the shot lands.
        int CheckShot(double angle) {

            int distance;
            //double angle;

            // Convert to radians.
            //angle = (angle * kPi) / 180.0;
            distance =  DistanceCalc(angle);
            return distance;
        }

        // Generate a random number for the enemy position.
        int Initialize() {
            int enemy_position;

            // Initialize random seed.
            srand (time(NULL));

            // Generate random number between kMinDist and kMaxDist
            enemy_position = rand() % kMaxDist + kMinDist;
            return enemy_position;
        }

        // This function plays the game
        int Fire() {
            int number_killed;
            int enemy, shots, hit;
            int distance;

            // Initialize variables.
            shots = kNumShells;
            enemy = Initialize();
            distance = 0;
            hit = 0;

            do {
                // Get the distance where shot lands & compare it to enemy position.
                distance = CheckShot(3.14);
                // Some error checking on the input.
                if (distance == -1) {
                    continue;
                }
                // Compare the enemy position with the computed distance.
                if (abs(enemy - distance) <= 1) {
                    hit = 1;
                    number_killed++;
                } else {
                    shots--;
                    /**
                    if (distance > enemy) {
                        cout << "You over shot by " << abs(enemy - distance) << endl;
                    } else {
                        cout << "You under shot by " << abs(enemy - distance) << endl;
                    }
                     */
                }
            } while ((shots > 0) && (!(hit)));
            return number_killed;
        }

        // This shows the introductory screen.
        /**
        int main() {
            
            int killed = 0;
            char game;
            
            StartUp();
            
            while (game != 'n') { 
                killed = Fire(killed);	
                cout << "Would you like to play again? (y/n): ";
                cin >> game;
            }
            
            
            cout << "You killed " << killed << " of the enemy." << endl;
            
            return 0;
        }
         */
    }
}