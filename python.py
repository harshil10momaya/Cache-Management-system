import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("stats.csv", names=["Metric", "Value"])
hits = int(df[df["Metric"] == "Cache Hits"]["Value"].sum())
misses = int(df[df["Metric"] == "Cache Misses"]["Value"].sum())

plt.pie([hits, misses], labels=["Hits", "Misses"], autopct="%1.1f%%", colors=["green", "red"])
plt.title("Cache Performance")
plt.savefig("cache_performance.png")
plt.show()
