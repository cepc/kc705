
import numpy as np

def create_fake_map():
    noise = np.random.rand(48, 16) * 0.01
    res = noise

    # generate one hit
    x, y = np.rint(np.random.multivariate_normal([20, 5], [[4, 0], [0, 4]], 1))[0].astype(int)
    res[x, y] = 1
    return res

# def create_discrete_map():
#     res = create_fake_map()
#     discrete = (res*255).astype(np.uint8)

#     return discrete

def create_binary_map():
    res = create_fake_map()

    threshold = 0.0099
    binary = np.where(res > threshold, 1, 0)

    return binary

def main():
    tmp = create_binary_map()
    print(tmp)

if __name__ == '__main__':
    main()
