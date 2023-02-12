/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package com.babypatients.babysim_testapp;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.text.SimpleDateFormat;
import javax.swing.JPanel;
import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.DateAxis;
import org.jfree.chart.axis.ValueAxis;
import org.jfree.chart.plot.XYPlot;
import org.jfree.data.time.DynamicTimeSeriesCollection;
import org.jfree.data.time.Second;

/**
 *
 * @author victor
 */
public class SensorGraph {

    public void init(JPanel jGraphFrameSens){
        jGraphFrameSens.setLayout(new java.awt.BorderLayout());
        try{
              dataset.setTimeBase(new Second(0, 0, 0, 11, 2, 2023));
              dataset.addSeries(new float[1], 0, "ToF");
              jChartSensor = ChartFactory.createTimeSeriesChart(
            "ToF", "Time", "ToF", dataset, true, true, false);
        final XYPlot plot = jChartSensor.getXYPlot();
        ValueAxis axis_val = plot.getRangeAxis();
        axis_val.setRange(0.0,26.0);
        
        DateAxis axis = (DateAxis) plot.getDomainAxis();
        axis.setFixedAutoRange(10000);
        axis.setDateFormatOverride(new SimpleDateFormat("ss.SS"));
        jSensorPlot = createGraphChartPanel(jChartSensor);
        setChartPanelParameters(jSensorPlot);
        jGraphFrameSens.add(jSensorPlot, BorderLayout.CENTER);
        jGraphFrameSens.validate();
        jGraphFrameSens.setVisible(true);
        } catch (Exception e) {
            System.out.print("Chart exception:" + e);
        }
    }
    public void setChartType(SensorType newType){
        type = newType;
        setGraphTitleandLegend(newType);
        setMeasurementValRange(newType);
    }
    public SensorType getChartType(){
        return type;
    }
    public void setGraphTitleandLegend(SensorType Sensor){
        if(Sensor == SensorType.sensor_type_ToF)
            setGraphTitleAndLegendForToF();
        else if(Sensor == SensorType.sensor_type_Flow)
            setGraphTitleAndLegendForFlow();
    }
    public void setMeasurementValRange(SensorType Sensor){
        if(Sensor == SensorType.sensor_type_ToF)
            setMeasurementValRangeForToF();
        else if(Sensor == SensorType.sensor_type_Flow)
            setMeasurementValRangeForFlow();
    }
    private void setGraphTitleAndLegendForToF(){
        jChartSensor.removeLegend();
        jChartSensor.setTitle("ToF");
    }
    private void setGraphTitleAndLegendForFlow(){
        jChartSensor.removeLegend();
        jChartSensor.setTitle("Flow");
    }

    private void setMeasurementValRangeForToF(){
        final XYPlot plot = jChartSensor.getXYPlot();
        ValueAxis axis_val = plot.getRangeAxis();
        axis_val.setRange(0.0,26.0);
        axis_val.setLabel("ToF");
    }

    private void setMeasurementValRangeForFlow(){
        final XYPlot plot = jChartSensor.getXYPlot();
        ValueAxis axis_val = plot.getRangeAxis();
        axis_val.setRange(-500,500);
        axis_val.setLabel("Flow");
    }

    private ChartPanel createGraphChartPanel(JFreeChart chart){
        return new ChartPanel(chart) {
            @Override
            public Dimension getPreferredSize() {
                return new Dimension(320, 240);
            }
            };
    }
        private void setChartPanelParameters(ChartPanel panel){
            panel.setMouseWheelEnabled(true);
    }
    public DynamicTimeSeriesCollection dataset = new DynamicTimeSeriesCollection(1, 100, new Second());
    private ChartPanel jSensorPlot;
    private JFreeChart jChartSensor;
    private SensorType type;
}
