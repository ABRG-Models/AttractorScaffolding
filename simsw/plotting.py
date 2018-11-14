import numpy as np
import pylab as pl

def HSVtoRGB(h,s,v):
    #HSV colour wheel, supply inputs in range 0,1
    i = np.floor(h*6)
    f = h*6.-i
    p = v*(1.-s)
    q = v*(1.-f*s)
    t = v*(1.-(1.-f)*s)
    imod = i%6
    if(imod==0): return p,q,v
    elif(imod==1): return t,p,v
    elif(imod==2): return v,p,q
    elif(imod==3): return v,t,p
    elif(imod==4): return q,v,p
    elif(imod==5): return p,v,t

def correl(x,y):
    df = len(x)-2
    SP = np.sum(x*y)-(np.sum(x)*np.sum(y))/len(x)
    SSx = np.sum(x**2)-(np.sum(x)**2)/len(x)
    SSy = np.sum(y**2)-(np.sum(y)**2)/len(y)
    m = SP/SSx
    c = np.mean(y)-m*np.mean(x)
    r,p=np.corrcoef(x,y)
    return m,c,r,p

P = np.array([0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5])

pref = 'nodrift/'
files = [pref+'full_0.05.bin',
         pref+'full_0.10.bin',
         pref+'full_0.15.bin',
         pref+'full_0.20.bin',
         pref+'full_0.25.bin',
         pref+'full_0.30.bin',
         pref+'full_0.35.bin',
         pref+'full_0.40.bin',
         pref+'full_0.45.bin',
         pref+'full_0.50.bin']
nf = len(files)

fs=16
scale = 1000.


F1 = pl.figure(figsize=(15,5))
f1 = F1.add_subplot(131)

M = np.zeros([nf,1])

for y,file in enumerate(files):
    D = np.fromfile(file,dtype='int32')*1.

    print (D)

    bins = np.linspace(1,0.5*np.max(D),10)
    h,b=np.histogram(D,bins)

    x = np.where(np.log(h)>0)[0]
    m,c,r,p = correl(b[x],np.log(h[x]))
    bx = np.array([b[x[0]],b[x[-1]]])
    by = m*bx+c
    M[y] = m
    f1.plot(bx/scale,by,'-',linewidth=1,color=HSVtoRGB((y+1)/len(P)*0.5,1,1))


DS = []
for y,file in enumerate(files):
    D = np.fromfile(file,dtype='int32')*1.0
    bins = np.linspace(1,0.5*np.max(D),10)
    h,b=np.histogram(D,bins)
    f1.plot(b[:-1]/scale,np.log(h),'.',color=HSVtoRGB((y+1)/len(P)*0.5,1,1))
    DS = np.hstack([DS, np.mean(D)])

leg = [r'$p=0.05$',
        r'$p=0.10$',
        r'$p=0.15$',
        r'$p=0.20$',
        r'$p=0.25$',
        r'$p=0.30$',
        r'$p=0.35$',
        r'$p=0.40$',
        r'$p=0.45$',
        r'$p=0.50$']

f1.legend(leg,frameon=False)
f1.set_ylim([0,8])
f1.set_ylabel(r'$\log({\rm evolutions})$',fontsize=fs)
f1.set_xlabel('1000 generations',fontsize=fs)
f1.set_xlim([0,140])
f1.set_aspect(np.diff(f1.get_xlim())/np.diff(f1.get_ylim()))
f1.set_axisbelow(True)

m,c,r,p = correl(P,np.log(-M.T))
Pplt = np.linspace(P[0],P[-1],1000)

f2 = F1.add_subplot(132)
f2.plot(P,M,'.',color=(0,0,0))
f2.plot(Pplt,-np.exp(m*Pplt+c),'-',linewidth=1,color=(0,0,0))
f2.set_xlabel(r'$p$',fontsize=fs)
f2.set_ylabel(r'$m$',fontsize=fs)
f2.set_aspect(np.diff(f2.get_xlim())/np.diff(f2.get_ylim()))

f3 = F1.add_subplot(133)
f3.plot(P,DS,'.',color=(0,0,0))
f3.set_xlabel(r'$p$',fontsize=fs)
f3.set_ylabel(r'$\mu$',fontsize=fs)
f3.set_aspect(np.diff(f3.get_xlim())/np.diff(f3.get_ylim()))

F1.tight_layout()
F1.savefig(pref+"Figure"+'.pdf', dpi=300, format='pdf')

pl.show()
