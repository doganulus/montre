import numpy as np
import pandas as pd

from montre.algebra import TimedRelation, Bound
#
# Timed behavior implementation based on Pandas' dataframes
# 

class TimedBehavior(object):
    """A container for timed behaviors based on Pandas' dataframes"""
    def __init__(self, dates, values, columns=None):
        super(TimedBehavior, self).__init__()

        self.df = pd.DataFrame(values, index=pd.to_datetime(dates), columns=columns, dtype="category")

    def merge(self, other):
        m = pd.concat([self.df, other.df], axis=1, ignore_index=True).fillna(method='bfill')
        return TimedBehavior(m.index, list(m.values), columns=list(self.df.columns)+list(other.df.columns))

    def __str__(self):
        return str(self.df)


    def to_timed_relation(self, column, category):
        """Naive builder"""
        
        Z = TimedRelation()
        begin, end = 0, 0
        # code = self.df[column].cat.categories.get_loc(category)
        # codes = self.df[column].cat.codes
        dates = self.df[column].index.view('int64')
        values = self.df[column].values
        active = False

        for i in np.arange(0, len(dates)):
            #print(i, dates[i], codes[i], values[i])
            if values[i] == category and active == False:
                active = True
                begin = dates[i-1]
            elif values[i] != category and active == True:
                active = False
                end = dates[i-1]
                Z.append(
                    bmin=Bound.geq(begin),
                    bmax=Bound.leq(end),
                    emin=Bound.geq(begin),
                    emax=Bound.leq(end),
                    dmin=Bound.gt(0)
                )

        if active:
            end = dates[-1]
            Z.append(
                    bmin=Bound.geq(begin),
                    bmax=Bound.leq(end),
                    emin=Bound.geq(begin),
                    emax=Bound.leq(end),
                    dmin=Bound.gt(0)
                )

        return Z

