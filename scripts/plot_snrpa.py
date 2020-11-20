import pandas as pd

import matplotlib.pyplot as plt

random = pd.read_csv('../results/random_100.csv', sep=';')
uct = pd.read_csv('../results/uct_1_100_60.csv', sep=';')
grave = pd.read_csv('../results/grave_100_.01_50_60.csv', sep=';')
snrpa_os = pd.read_csv('../results/snrpa_100_3_1_50_100_os.csv', sep=';')
snrpa = pd.read_csv('../results/snrpa_100_2_1_50_100.csv', sep=';')
nrpa = pd.read_csv('../results/nrpa_100_2_25_1.csv', sep=';')

datasets = [
    random,
    uct,
    grave,
    snrpa_os,
    snrpa,
    nrpa,
]

labels = [
    'random',
    'uct',
    'grave',
    'snrpa_os',
    'snrpa',
    'nrpa',

]

positions = list(range(len(datasets)))

fig, ax = plt.subplots()

data = []
avg_highest_reward = []
avg_lowest_reward = []

for dataset in datasets:
    data.append(dataset['reward'])
    avg_highest_reward.append(dataset['highest_reward'].mean())
    avg_lowest_reward.append(dataset['lowest_reward'].mean())

ax.boxplot(data, labels=labels, positions=positions, showfliers=False)
ax.scatter(positions, avg_highest_reward, label='Average highest reward', marker='_', s=200)
ax.scatter(positions, avg_lowest_reward, label='Average lowest reward', marker='_', s=200)

ax.legend(loc='upper center', bbox_to_anchor=(0.5, 1.05),
          ncol=3, fancybox=True, shadow=True)

fig.savefig('test.png')

