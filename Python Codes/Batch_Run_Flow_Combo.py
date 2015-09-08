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
    simulator=GAimsunSimulator()
    simulator.setModel( model )
    experiment = replication.getExperiment()
    if experiment.getEngineMode()==GKExperiment.eOneShot:
	print "is oneshot"
        if replication.getSimulationStatus()==GKGenericExperiment.ePending or True:
	    print "is pending"
            task=GKSimulationTask()
            task.replication=replication
            task.mode=GKReplication.eBatch
            simulator.addSimulationTask( task )
	    print "simulate"
	    resultNow = simulator.simulate()
	    print "result: " + str(resultNow)
	    print "has simulated"

def handleArguments(fileName, expId):
    print "opening console"
    console = ANGConsole()
    if console != None:
        print "console opened"
    else:
        print "no console"
        return

    print "opening file"
    if console.open(fileName):
        print "file opened"
        model = console.getModel()
        print "find experiment %s in catalog" % expId
        result = model.getCatalog().find(expId)
        'read flow combination'
        on_ramp = []
        off_ramp = []
        through = []
        if readflow(on_ramp, off_ramp, through):
            for on_ramp_flow in xrange(on_ramp[0], on_ramp[2], (on_ramp[1] if on_ramp[1]>0 else 1)):
                for off_ramp_flow in xrange(off_ramp[0], off_ramp[2], (off_ramp[1] if off_ramp[1]>0 else 1)):
                    for through_flow in xrange(through[0], through[2],(through[1] if through[1]>0 else 1)):
                        savetempfile(on_ramp_flow, off_ramp_flow, through_flow)
                        print "On_ramp: %d, Off_ramp: %d, and Through: %d" % (on_ramp_flow, off_ramp_flow, through_flow)
                        simulateMicro(model, result)
        else:
            print('Invalid flow file!')
        console.save(fileName)
        console.close()
    else:
        print "cannot load"
        console.getLog().addError("Cannot load the network")
        console.close()

def readflow(on_ramp,off_ramp,through):
    text_file = open("C:\CACC_Simu_Data\Batch_Volume.txt", "r")
    data = text_file.read().replace('\n', '')
    text_file.close()
    val = data.split(',')
    try:
        if len(val) != 9:
            return False
        for i in xrange(3):
            on_ramp.append(int(val[i]))
        if on_ramp[0] >= on_ramp[2]:
            on_ramp[0]=0
            on_ramp[1]=1
            on_ramp[2]=1
        for i in xrange(3, 6):
            through.append(int(val[i]))
        if through[0] >= through[2]:
            through[0]=0
            through[1]=1
            through[2]=1
        for i in xrange(6, 9):
            off_ramp.append(int(val[i]))
        if off_ramp[0] >= off_ramp[2]:
            off_ramp[0]=0
            off_ramp[1]=1
            off_ramp[2]=1
        print val
    except ValueError:
        return False
    return True

def savetempfile(on_ramp_flow,off_ramp_flow, through_flow):
    text_file = open("C:\CACC_Simu_Data\Temp.txt", "w")
    content = '{0},{1},{2}'.format(on_ramp_flow, through_flow, off_ramp_flow)
    text_file.write(content)
    text_file.close()

def main(argv):
    print "inside here"
    for x in argv:
        print x
    if len(argv) < 3:
        print "usage: %s ANG_FILE_NAME EXPERIMENT_ID " % argv[0]
        return -1

    fileName = str(argv[1])
    expId = int(argv[2])

    handleArguments(fileName, expId)
    print "handledArguments"

if __name__ == "__main__":
    sys.exit(main(sys.argv))
