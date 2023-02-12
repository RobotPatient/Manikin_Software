/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package com.babypatients.babysim_testapp;

import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortDataListener;
import com.fazecast.jSerialComm.SerialPortEvent;
import org.jfree.data.time.DynamicTimeSeriesCollection;


enum SensorType{
    sensor_type_Flow, sensor_type_ToF
}

class SensorResult{
    public short parseResult(byte lowerhalf, byte upperhalf){
        return (short)((lowerhalf & 0xFF )| ((upperhalf & 0xFF) << 8));
    }
    
    public boolean ValidateAndAddToDataset(DynamicTimeSeriesCollection dataset, short result){
        if(sensType == SensorType.sensor_type_ToF && ( result == 0 || result > 255) )
            return false;
        else if (sensType == SensorType.sensor_type_ToF){
            resultFloat[0] = result/10;
        }
        else{
            resultFloat[0] = result;
        }
        dataset.advanceTime();
        dataset.appendData(resultFloat);
        return true;
    }
    SensorType sensType;
    float resultFloat[] = new float[1];
    
}

class Serial_event_listener implements SerialPortDataListener{
      @Override
      public int getListeningEvents() { return SerialPort.LISTENING_EVENT_DATA_AVAILABLE; }
      @Override
      public void serialEvent(SerialPortEvent event)
      {
        if (event.getEventType() != SerialPort.LISTENING_EVENT_DATA_AVAILABLE)
            return;
        selectedSerialPort.readBytes(newData, 7);
        resSensor1.sensType = typeSensor1;
        resSensor2.sensType = typeSensor2;
        if(newData[0] == 0 && newData[1] == 1){
              if(typeSensor1 == SensorType.sensor_type_Flow)
                sens1Result = resSensor1.parseResult(newData[2], newData[3]);
              else
                 sens1Result = resSensor1.parseResult(newData[4], newData[5]);
              if(typeSensor2 == SensorType.sensor_type_Flow)
                sens2Result = resSensor2.parseResult(newData[2], newData[3]);
              else
                 sens2Result = resSensor2.parseResult(newData[4], newData[5]);
              resSensor1.ValidateAndAddToDataset(datasetSens1, sens1Result);
              resSensor2.ValidateAndAddToDataset(datasetSens2, sens2Result);
              System.out.println(sens2Result);
        } 
      }
    byte[] newData = new byte[7];
    short sens1Result =0;
    short sens2Result = 0;
    SensorResult resSensor1 = new SensorResult();
    SensorResult resSensor2 = new SensorResult();
    DynamicTimeSeriesCollection datasetSens1;
    DynamicTimeSeriesCollection datasetSens2;
    SensorType typeSensor1;
    SensorType typeSensor2;
    SerialPort selectedSerialPort;
}
/**
 *
 * @author victor
 */
public class SerialInterpreter {
    
}
