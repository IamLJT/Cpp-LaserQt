import plotly.plotly as py
import plotly.graph_objs as go

f = open('estimators.txt')
Z = [[float(line.strip().split(', ')[0]), float(line.strip().split(', ')[1]), float(line.strip().split(', ')[2])] for line in f]
f.close()
trace = go.Heatmap(z=Z)
py.iplot([trace], filename='basic-heatmap')
