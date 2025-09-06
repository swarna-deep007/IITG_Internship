# Near-Popular Roommate Matching

This project implements a **novel variation** of the Roommate Matching Problem, inspired by the Gale–Shapley Stable Matching algorithm, but adapted to **non-bipartite roommate graphs** using the notion of **popularity** instead of stability. The approach recursively explores induced subgraphs and attempts to minimize *unpopularity factors*.

---

## 🔬 Research Context

- **Classical Gale–Shapley**: Works for bipartite stable matchings (e.g., marriage problem).  
- **Roommate Problem**: Involves non-bipartite graphs where stability often cannot be guaranteed.  
- **Near-Popular Matchings**: Instead of perfect stability (impossible in many cases), the algorithm seeks matchings with minimized *unpopularity*.  

This implementation is based on the theoretical foundations of NP-hardness proofs and graph decomposition methods in matching theory.

---

## 📚 What This Project Does

- Represents the roommate problem as a **graph** (`Graph` class).  
- Implements a **modified Gale–Shapley** matching between two dynamically chosen partitions of vertices.  
- Recursively applies **graph decomposition** on unmatched vertices to construct a near-popular solution.  
- Randomized **stable partitioning** ensures variability across runs.  
- Outputs a set of roommate matchings.

---

## ⚙️ How It Works

1. Generate a random graph with `n` vertices (edges exist with some probability).  
2. Partition the vertices randomly into groups (`stablePartition`).  
3. Apply **Gale–Shapley–style proposals** between subsets (`galeShapley`).  
4. Collect matchings, recurse on leftover vertices (`unpopularityMatching`).  
5. Return a near-popular matching set.

---

## ▶️ Example Run

```bash
g++ -std=c++17 roommate_matching.cpp -o roommate_matching
./roommate_matching
