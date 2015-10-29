__author__ = 'Dali'

import sys
from PyANGBasic import *
from PyANGKernel import *
from PyANGConsole import *
from PyANGAimsun import *
from PyANGDTA import *

from time import localtime, strftime

def simulateMicro(model, replication):
    print "simulateMicro"
    plugin = GKSystem.getSystem().getPlugin( "GGetramModule" ) # GGetramModule
    simulator = GAimsunSimulator()
    simulator.setModel(model)
    experiment = replication.getExperiment()
    if experiment.getEngineMode() == GKExperiment.eOneShot:
        print "is oneshot"
        task = GKSimulationTask()
        task.replication=replication
        task.mode = GKReplication.eBatch
        simulator.addSimulationTask( task )
        print "simulate"
        resultNow = simulator.simulate()
        print "result: " + str(resultNow)
        print "has simulated"

def handleArguments(filename, exp_id):
    print "opening console"
    console = ANGConsole()
    if console is not None:
        print "console opened"
    else:
        print "no console"
        return

    print "opening file"
    if console.open(filename):
        print "file opened"
        model = console.getModel()
        print "find experiment %s in catalog" % exp_id
        result = model.getCatalog().find(exp_id)
        'read flow combination'
        simulateMicro(model, result)
        console.close()
    else:
        print "cannot load"
        console.getLog().addError("Cannot load the network")
        console.close()

def main(argv):
    print "main"
    for x in argv:
        print x
    if len(argv) < 3:
        print "usage: %s ANG_FILE_NAME EXPERIMENT_ID " % argv[0]
        return -1

    filename = str(argv[1])
    exp = int(argv[2])

    handleArguments(filename, exp)
    print "handledArguments"

if __name__ == "__main__":
    sys.exit(main(sys.argv))
