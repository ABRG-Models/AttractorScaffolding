# Some analysis code, collected into a library.
import numpy

class Analysis():

    def __init__(self):
        print ('init')

    # Sample, with replacement, from range 0, nRows.
    def bootstrap_sample (n):
        indices = numpy.floor((n * numpy.random.ranf([1,n])));
        return indices.astype(int)

    # Compute the mean of the distribution and the standard error by
    # the bootstrap method. Also return the 95% confidence interval
    # range (or the conf. int. specified by @ci
    def bootstrap_mean (distribution, ci=95, nSamp=1024):
        d_arr = numpy.array(distribution)
        bmeans = numpy.zeros (nSamp)
        nRows = len(d_arr)
        for s in range (0, nSamp):
            bs = Analysis.bootstrap_sample (nRows)
            bmeans[s] = numpy.mean (d_arr[bs])
        std_err = numpy.sqrt(numpy.var(bmeans))

        # Sort bmeans to generate the percentiles.
        bmeans_srt = numpy.sort (bmeans);
        # nSamp is length of bmeans and bmeans_srt
        if ci < 0.0 or ci > 100.0:
            print ('Invalid confidence interval {0}, forcing it to 95'.format(ci))
            ci = 95
        cirest = 100 - ci
        cileft = cirest/2.0
        ciright = ci + cileft
        print ('Conf. int is {0} from {1} to {2}'.format(ci, cileft, ciright))
        t2p5 = int(round((nSamp/100.0) * cileft))
        n97p5 = int(round((nSamp/100.0) * ciright))
        print ('That''s index {0} to {1}'.format(t2p5, n97p5))
        confint = [bmeans_srt[t2p5], bmeans_srt[n97p5]]

        return numpy.mean(d_arr), std_err, confint
