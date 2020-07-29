import numpy as np
import csv

g = 9.8
L = 2
mu = 0.1

THETA_0 = np.pi / 3
THETA_DOT_0 = 0


def get_theta_double_dot(theta, theta_dot):
    return -mu * theta_dot - (g/L) * np.sin(theta)


def theta(t):
    global THETA_DOT_0
    global THETA_0
    theta = THETA_0
    theta_dot = THETA_DOT_0
    delta_t = 0.001
    for time in np.arange(0, t, delta_t):
        theta_double_dot = get_theta_double_dot(
            theta, theta_dot
        )
        theta += theta_dot * delta_t
        theta_dot += theta_double_dot * delta_t
        with open('VectorFiled3.csv', 'a') as fd:
            fd.write(str(theta) + ',' + str(theta_dot) +
                     ',' + str(theta_double_dot) + '\n')
        print("Theta: ", theta, " |||| Theta_dot: ", theta_dot,
              " |||| Theta_double_dot: ", theta_double_dot)
    return theta


def main():
    print("###Vector field calculator###\n")
    theta(30)


# Start point of the application
if __name__ == "__main__":
    main()
