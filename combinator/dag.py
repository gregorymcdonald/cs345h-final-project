class DAG:

    def __init__(self, root):
        self.graph = {}
        self.root = root

    def add(self, src, dest):
        s = self.graph.get(src, set())
        s.add(dest)
        self.graph[src] = s

    def get_topological_order(self):
        order = []
        in_progress = set()
        done = set()
        self.post_order_dfs(self.root, order, in_progress, done)
        return order

    def post_order_dfs(self, current, order, in_progress, done):
        in_progress.add(current)
        for out in self.graph.get(current, set()):
            if out in in_progress:
                raise CycleError(current, out)
            self.post_order_dfs(out, order, in_progress, done)
        if current not in done:
            order.append(current)
        done.add(current)
        in_progress.discard(current)

class CycleError(Exception):

    def __init__(self, n1, n2):
        self.n1 = n1
        self.n2 = n2

