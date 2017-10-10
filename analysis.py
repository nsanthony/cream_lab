import numpy as np
from numpy import genfromtxt
import matplotlib.pyplot as plt
import pymc
import random
#Latest: Time (s)
#Latest: Potential (mV)
#Latest: Potential 1 (V)
#Latest: Potential 2 (V)
#Latest: Illumination (lux)
wave = {0:'465',1:'560',2:'585',3:'635',4:'700',5:'810'}
s = 0
path_lum = '405diodedata/luminosity_'+wave[s]+'.csv'
path_spectrum = '405diodedata/spectrum_'+wave[s]+'.csv'
V_Lu = genfromtxt(path_lum,delimiter=',')
spec = genfromtxt(path_spectrum,delimiter=',')

plt.figure(1)
plt.plot(V_Lu[1:,3],V_Lu[1:,4])

plt.figure(2)
plt.plot(spec[1:,0],spec[1:,1])
plt.show()


k = len(V_Lu)
x_data = V_Lu[1:,3]
y_data = V_Lu[1:,4]
alpha = 650/0.5
beta = -800

x = pymc.Normal('x', mu=0,tau=1,value=x_data, observed=True)
@pymc.deterministic(plot=False)
def linear_regress(x=x, alpha=alpha, beta=beta):
    return x*alpha+beta

y = pymc.Normal('output', mu=linear_regress, value=y_data, observed=True)

model = pymc.Model([x, y, alpha, beta])
mcmc = pymc.MCMC(model)
mcmc.sample(iter=100000, burn=1, thin=10)

a = mcmc.stats('alpha')
b = mcmc.stats('beta')

#print()
#print('Alpha mean: ', a['alpha']['mean'])
#print('Alpha std: ',a['alpha']['standard deviation'])
#print('Beta mean: ',b['beta']['mean'])
#print('Beta std: ',b['beta']['standard deviation'])
#print()
#
#ta = mcmc.trace('alpha')[:]
#tb = mcmc.trace('beta')[:]
#x = np.linspace(1,9000,9000)
#plt.figure(1)
#plt.plot(x,ta,'.')
#plt.title('Alpha Trace')
#plt.figure(2)
#plt.title('Histogram of Alpha Trace')
#plt.hist(ta, bins=100)
#plt.figure(3)
#plt.title('Beta Trace')
#plt.plot(x,tb,'.')
#plt.figure(4)
#plt.title('Histogram of Beta Trace')
#plt.hist(tb,bins=100)