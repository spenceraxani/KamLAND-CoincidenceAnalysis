#!/usr/bin/python
 
import math
 
def smooth_points(events, width, samples=500, normalize_to_match_bucket_count=None):
    start, stop = min(events), max(events)
    xs, ys = [], []
    for i in xrange(samples):
        t = start + i / (samples - 1.0) * (stop - start)
        value = 0.0
        for event in events:
            value += math.exp(-(t - event)**2 / (2.0 * width**2.0))
        xs.append(t)
        ys.append(value)
    if normalize_to_match_bucket_count != None:
        # If we have a specific number of buckets then your average uniform sample contributes 1/bucket_count to a given bucket.
        # For a bucket in the middle, the average uniform sample contributes Integrate[E^(-x^2/(2*width^2)), {x, -size/2, size/2}] / size
        size = stop - start
        normalization_constant = normalize_to_match_bucket_count * (2 * math.pi)**0.5 * width * math.erf(size / (2**1.5 * width)) / size
        ys = [y / normalization_constant for y in ys]
    return xs, ys
 
if __name__ == "__main__":
    # Choose some random points according to a normal distribution.
    import random
    events = [random.normalvariate(random.choice((250, 750)), 100) for _ in xrange(100)]
    #events = [random.uniform(0, 100) for _ in xrange(5000)]
    bucket_count = 20
 
    xs, ys = smooth_points(events, 30, normalize_to_match_bucket_count=bucket_count)
 
    from matplotlib import pyplot as plt
    plt.hold(True)
    plt.hist(events, bucket_count)
    plt.plot(xs, ys, lw=3)
    plt.scatter(events, [random.uniform(-5, 0) for _ in xrange(len(events))])
    plt.show()
