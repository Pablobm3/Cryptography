
total_balls = 2000


def dupl_prob(total_balls):
    probability = 1.0
    for i in range(1, total_balls + 1):
        probability *= (total_balls - i + 1) / total_balls
        if 1 - probability >= 0.5:
            return i
    return total_balls


min = dupl_prob(total_balls)
print("The minimum number of balls necessary to have at least a 50% of probability to repeat ball is: ", min)


def dupl_prob1(total_balls):
    probability = 1.0
    for i in range(1, total_balls + 1):
        probability *= (total_balls - 1) / total_balls
        if 1 - probability >= 0.5:
            return i+1
    return total_balls


min1 = dupl_prob1(total_balls)
print("The minimum number of balls necessary to have at least a 50% of probability to repeat ball is: ", min1)




