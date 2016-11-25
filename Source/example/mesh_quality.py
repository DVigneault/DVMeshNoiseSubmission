#!/usr/bin/env python3

import matplotlib

font = {'family' : 'sans-serif',
        'weight' : 'normal',
        'size'   : 15,
       }
matplotlib.rc('font', **font)

import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt
import pandas as pd
from scipy import stats

def PlotMeshQuality(metric,
                    bins,
                    title,
                    x_label,
                    ):
  noise_file = '../data/noise_' + metric + '.txt'
  clean_file = '../data/clean_' + metric + '.txt'
  output_file = '../data/' + metric + '.png'

  noise = pd.read_csv(noise_file).as_matrix().flatten()
  clean = pd.read_csv(clean_file).as_matrix().flatten()

  fig = plt.figure(figsize = (10, 10))

  top = fig.add_subplot(211)
  for d in ['top', 'bottom', 'left', 'right']: top.spines[d].set_color('none')
  top.tick_params(labelcolor='w', top='off', bottom='off', left='off', right='off')
  top.set_xlabel(x_label)

  btm = fig.add_subplot(212)
  for d in ['top', 'bottom', 'left', 'right']: btm.spines[d].set_color('none')
  btm.tick_params(labelcolor='w', top='off', bottom='off', left='off', right='off')
  btm.set_xlabel('Theoretical Quantiles')

  ax1 = fig.add_subplot(2,2,1)
  ax2 = fig.add_subplot(2,2,2)
  ax3 = fig.add_subplot(2,2,3)
  ax4 = fig.add_subplot(2,2,4)

  ## Histograms

  if (None == bins):
    clean_bins = ((max(clean) - min(clean)) + 1)
    noise_bins = ((max(noise) - min(noise)) + 1)
    ax1.hist(clean, clean_bins, normed=1, facecolor="#df5e83", alpha=0.75)
    ax2.hist(noise, noise_bins, normed=1, facecolor="#df5e83", alpha=0.75)
  else:
    ax1.hist(clean, bins, normed=1, facecolor="#df5e83", alpha=0.75)
    ax2.hist(noise, bins, normed=1, facecolor='#df5e83', alpha=0.75)
  
  ## Probability Plot

  ((osm_c, osr_c), (m_c, b_c, r_c)) = stats.probplot(clean)
  ((osm_n, osr_n), (m_n, b_n, r_n)) = stats.probplot(noise)

  ax3.plot(osm_c, osr_c, 'o', color='green')
  ax3.annotate('$R^2 = %.2f$'%(r_c**2), xy=(0.03,0.9), xycoords='axes fraction')
  ax4.plot(osm_n, osr_n, 'o', color='green')
  ax4.annotate('$R^2 = %.2f$'%(r_n**2), xy=(0.03,0.9), xycoords='axes fraction')

  ## Save

  fig.suptitle(title, fontsize=18, y = 0.97)
  ax1.set_ylabel('Counts (Normalized)')
  ax3.set_ylabel('Values (Ordered)')
  fig.tight_layout(rect=[0, 0, 1, 0.9])
  fig.savefig(output_file)

###################
## Aspect Ratios ##
###################

metric = 'ratios'
bins = 50
title = 'Aspect Ratios Before (Left) and After (Right)\nPerturbation with Gaussian Noise'
x_label = 'Aspect Ratios of Mesh Triangle Faces'

PlotMeshQuality(metric,
                bins,
                title,
                x_label,
               )

###########
## Areas ##
###########

metric = 'areas'
bins = 50
title = 'Areas Before (Left) and After (Right)\nPerturbation with Gaussian Noise'
x_label = 'Areas of Mesh Triangle Faces'

PlotMeshQuality(metric,
                bins,
                title,
                x_label,
               )

###############
## Valencies ##
###############

metric = 'valencies'
bins = None
title = 'Valencies Before (Left) and After (Right)\nPerturbation with Gaussian Noise'
x_label = 'Valencies of Mesh Vertices'

PlotMeshQuality(metric,
                bins,
                title,
                x_label,
               )

