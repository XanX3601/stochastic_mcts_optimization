import pandas as pd

import matplotlib.pyplot as plt

random = pd.read_csv('../results/snrpa_100_1_1_25_1000.csv', sep=';')
uct = pd.read_csv('../results/snrpa_100_1_1_25_100.csv', sep=';')

datasets = [
    random,
    uct,
]

labels = [
    'snrpa 100 playouts',
    'snrpa 1000 playouts'
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

