from pvaccess import UINT, PvaServer, PvObject
import time

pv_server = PvaServer()
pv_running = PvObject({'value': UINT}, {'value': 1})
pv_server.addRecord('FTEST03:RUNNING', pv_running)

pv_dev1 = PvObject({'value': UINT}, {'value': 0})
pv_server.addRecord('FTEST03:DEV1-STATUS', pv_dev1)

pv_dev2 = PvObject({'value': UINT}, {'value': 0})
pv_server.addRecord('FTEST03:DEV2-STATUS', pv_dev2)

pv_status = PvObject({'value': UINT}, {'value': 0})
pv_server.addRecord('FTEST03:DEVICES-READY', pv_status)

while True:
  print("   running:{0:n} dev1:{1:n} dev2:{2:n} status:{3:n}".format(
    pv_running["value"],
    pv_dev1["value"],
    pv_dev2["value"],
    pv_status["value"]))
  time.sleep(3)
  