from lib import rand_range

def integral_hom (f, min_x, max_x, max_y, N_points) :
    x_points = [rand_range (min_x, max_x) for i in range (N_points)]
    y_points = [rand_range (0, max_y) for i in range (N_points)]

    counts = 0
    for x, y in zip (x_points, y_points):
      if y <= f (x) : counts += 1

    p = counts / N_points
    A = max_y * (max_x - min_x)
    return A * p, A**2 * p * (1-p) / N_points

