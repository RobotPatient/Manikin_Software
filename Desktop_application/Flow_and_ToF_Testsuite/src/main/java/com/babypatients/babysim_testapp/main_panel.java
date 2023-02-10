/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/GUIForms/JFrame.java to edit this template
 */
package com.babypatients.babysim_testapp;
import com.fazecast.jSerialComm.*;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.nio.CharBuffer;
import java.nio.charset.StandardCharsets;
import java.text.SimpleDateFormat;
import java.util.Base64;
import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.ChartUtils;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.DateAxis;
import org.jfree.chart.axis.ValueAxis;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.plot.XYPlot;
import org.jfree.data.statistics.HistogramDataset;
import org.jfree.data.time.DynamicTimeSeriesCollection;
import org.jfree.data.time.Second;
import org.jfree.data.xy.Vector;
import org.jfree.data.xy.XYDataset;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;
/**
 *
 * @author victor
 */
public class main_panel extends javax.swing.JFrame {

    /**
     * Creates new form main_panel
     */
    public main_panel() {
        initComponents();
        initGraphs();
    }

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jComLabel = new javax.swing.JLabel();
        jBaudLabel = new javax.swing.JLabel();
        jBaudRateField = new javax.swing.JTextField();
        jConnectButton = new javax.swing.JButton();
        jGraphFrameSens1 = new javax.swing.JPanel();
        jGraphFrameSens2 = new javax.swing.JPanel();
        jSensorTypeSens1 = new javax.swing.JComboBox<>();
        jSensorTypeLabelSens2 = new javax.swing.JLabel();
        jSensor1Label = new javax.swing.JLabel();
        jSensor2Label = new javax.swing.JLabel();
        jSensorTypeLabelSens1 = new javax.swing.JLabel();
        jSensorTypeSens2 = new javax.swing.JComboBox<>();
        jConnectionStatus = new javax.swing.JLabel();
        jConnectionStatusLabel = new javax.swing.JLabel();
        jComChooser = new javax.swing.JComboBox<>();
        jDisconnectButton = new javax.swing.JButton();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);

        jComLabel.setText("COM:");

        jBaudLabel.setText("BAUD:");

        jBaudRateField.setText("115200");

        jConnectButton.setText("Connect");
        jConnectButton.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jConnectButtonMouseClicked(evt);
            }
        });
        jConnectButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jConnectButtonActionPerformed(evt);
            }
        });

        jGraphFrameSens1.setBorder(javax.swing.BorderFactory.createBevelBorder(javax.swing.border.BevelBorder.RAISED));

        javax.swing.GroupLayout jGraphFrameSens1Layout = new javax.swing.GroupLayout(jGraphFrameSens1);
        jGraphFrameSens1.setLayout(jGraphFrameSens1Layout);
        jGraphFrameSens1Layout.setHorizontalGroup(
            jGraphFrameSens1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 0, Short.MAX_VALUE)
        );
        jGraphFrameSens1Layout.setVerticalGroup(
            jGraphFrameSens1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 214, Short.MAX_VALUE)
        );

        jGraphFrameSens2.setBorder(javax.swing.BorderFactory.createBevelBorder(javax.swing.border.BevelBorder.RAISED));

        javax.swing.GroupLayout jGraphFrameSens2Layout = new javax.swing.GroupLayout(jGraphFrameSens2);
        jGraphFrameSens2.setLayout(jGraphFrameSens2Layout);
        jGraphFrameSens2Layout.setHorizontalGroup(
            jGraphFrameSens2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 0, Short.MAX_VALUE)
        );
        jGraphFrameSens2Layout.setVerticalGroup(
            jGraphFrameSens2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 0, Short.MAX_VALUE)
        );

        jSensorTypeSens1.setModel(new javax.swing.DefaultComboBoxModel<>(new String[] { "ToF", "Flow" }));

        jSensorTypeLabelSens2.setText("Sensor type:");

        jSensor1Label.setText("Sensor 1:");

        jSensor2Label.setText("Sensor 2:");

        jSensorTypeLabelSens1.setText("Sensor type:");

        jSensorTypeSens2.setModel(new javax.swing.DefaultComboBoxModel<>(new String[] { "Flow", "ToF" }));

        jConnectionStatus.setBackground(new java.awt.Color(255, 255, 255));
        jConnectionStatus.setText("Not connected");
        jConnectionStatus.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        jConnectionStatus.setOpaque(true);

        jConnectionStatusLabel.setText("Status:");

        jComChooser.addAncestorListener(new javax.swing.event.AncestorListener() {
            public void ancestorAdded(javax.swing.event.AncestorEvent evt) {
                jComChooserAncestorAdded(evt);
            }
            public void ancestorMoved(javax.swing.event.AncestorEvent evt) {
            }
            public void ancestorRemoved(javax.swing.event.AncestorEvent evt) {
            }
        });

        jDisconnectButton.setText("Disconnect");
        jDisconnectButton.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jDisconnectButtonMouseClicked(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                .addGap(21, 21, 21)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jGraphFrameSens1, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(jSensorTypeLabelSens1, javax.swing.GroupLayout.DEFAULT_SIZE, 188, Short.MAX_VALUE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jSensorTypeSens1, 0, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)))
                .addGap(49, 49, 49)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(jGraphFrameSens2, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(jSensorTypeLabelSens2, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jSensorTypeSens2, 0, 187, Short.MAX_VALUE)))
                .addGap(55, 55, 55))
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jComLabel)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jComChooser, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jBaudLabel)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jBaudRateField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(30, 30, 30)
                .addComponent(jConnectButton)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jDisconnectButton)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 33, Short.MAX_VALUE)
                .addComponent(jConnectionStatusLabel)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jConnectionStatus)
                .addContainerGap())
            .addGroup(layout.createSequentialGroup()
                .addGap(38, 38, 38)
                .addComponent(jSensor1Label, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGap(166, 166, 166)
                .addComponent(jSensor2Label, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGap(143, 143, 143))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jSensor1Label, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(jSensor2Label, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jGraphFrameSens1, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(jGraphFrameSens2, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(12, 12, 12)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(jSensorTypeLabelSens2, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                            .addComponent(jSensorTypeSens2)))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(9, 9, 9)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(jSensorTypeSens1)
                            .addComponent(jSensorTypeLabelSens1, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))))
                .addGap(40, 40, 40)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jConnectionStatus, javax.swing.GroupLayout.PREFERRED_SIZE, 27, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                        .addComponent(jComLabel)
                        .addComponent(jBaudLabel)
                        .addComponent(jBaudRateField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addComponent(jConnectButton)
                        .addComponent(jConnectionStatusLabel)
                        .addComponent(jComChooser, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addComponent(jDisconnectButton)))
                .addContainerGap())
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents
    private void initGraphs(){
            jGraphFrameSens1.setLayout(new java.awt.BorderLayout());
        try{
              datasetSens1 = new DynamicTimeSeriesCollection(1, 10000, new Second());
              datasetSens1.setTimeBase(new Second(0, 0, 0, 23, 1, 2014));
              datasetSens1.addSeries(new float[1], 0, "ToF");
              jChartSensor1 = ChartFactory.createTimeSeriesChart(
            "ToF", "Time", "ToF", datasetSens1, true, true, false);
        final XYPlot plot = jChartSensor1.getXYPlot();
        ValueAxis axis_val = plot.getRangeAxis();
        axis_val.setRange(0.0,26.0);
        
        DateAxis axis = (DateAxis) plot.getDomainAxis();
        axis.setFixedAutoRange(10000);
        axis.setDateFormatOverride(new SimpleDateFormat("ss.SS"));
        jSensorPlot1 = new ChartPanel(jChartSensor1) {

            @Override
            public Dimension getPreferredSize() {
                return new Dimension(320, 240);
            }
            };
            jSensorPlot1.setMouseWheelEnabled(true);
            add(jSensorPlot1);
            setDefaultCloseOperation(EXIT_ON_CLOSE);
            pack();

            jGraphFrameSens1.add(jSensorPlot1, BorderLayout.CENTER);
            jGraphFrameSens1.validate();
            pack();
            jGraphFrameSens1.setVisible(true);

        } catch (Exception e) {
            System.out.print("Chart exception:" + e);
        }
    }
    private void jConnectButtonMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jConnectButtonMouseClicked
        // TODO add your handling code here:
        int SelectedIndex = jComChooser.getSelectedIndex();
        if(selectedSerialPort != null){
            if(selectedSerialPort.isOpen()){
                System.out.println("Disconnecting Port: "+selectedSerialPort.getDescriptivePortName());
                selectedSerialPort.closePort();
            }
        }
        selectedSerialPort = availableSerialPorts[SelectedIndex];
        if(selectedSerialPort.openPort()){
            jConnectionStatus.setText("Connected!");
            jConnectionStatus.setBackground(Color.green);
            String baudrateFieldText = jBaudRateField.getText();
            try{
            int baudrate = Integer.parseInt(baudrateFieldText);
            selectedSerialPort.setBaudRate(baudrate);
            selectedSerialPort.addDataListener(new SerialPortDataListener() {
            char[] Flow = new char[6];
            char[] ToF = new char[4];
            boolean FirstString = true;
            int iter2 = 0;
             @Override
             public int getListeningEvents() { return SerialPort.LISTENING_EVENT_DATA_AVAILABLE; }
             @Override
             public void serialEvent(SerialPortEvent event)
              {
                if (event.getEventType() != SerialPort.LISTENING_EVENT_DATA_AVAILABLE)
                 return;
                byte[] newData = new byte[selectedSerialPort.bytesAvailable()];
                int numRead = selectedSerialPort.readBytes(newData, newData.length);
                for(char p : ToF)
                    p ='\0';
                for(int i =0; i< newData.length-1; i++){
                    if((char)newData[i] != ',' && FirstString)
                        Flow[i] = (char)newData[i];
                    else if ( (char)newData[i] != ',' && !FirstString)
                        ToF[iter2++] = (char)newData[i];
                    else
                        FirstString = false;
                }
                iter2 = 0;
                FirstString = true;
                ToF[3] = '\0';
                String number = new String(ToF);
                int val = Integer.parseInt(number.trim(), 10); 
                float newDataFloat[] = new float[1];
                newDataFloat[0] = val/10;
                datasetSens1.advanceTime();
                datasetSens1.appendData(newDataFloat);
                
              }
              });
              }
            catch (NumberFormatException ex){
                ex.printStackTrace();
            }     
        }
        else{
            jConnectionStatus.setText("Fail!");
            jConnectionStatus.setBackground(Color.red);
        }
    }//GEN-LAST:event_jConnectButtonMouseClicked

    private void jComChooserAncestorAdded(javax.swing.event.AncestorEvent evt) {//GEN-FIRST:event_jComChooserAncestorAdded
        availableSerialPorts = SerialPort.getCommPorts();
        String PortName;
        for (SerialPort availableSerialPort : availableSerialPorts) {
           PortName = availableSerialPort.getSystemPortName() + ": " + availableSerialPort.getDescriptivePortName();
           jComChooser.addItem(PortName);
        }
    }//GEN-LAST:event_jComChooserAncestorAdded

    private void jConnectButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jConnectButtonActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_jConnectButtonActionPerformed

    private void jDisconnectButtonMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jDisconnectButtonMouseClicked
        // TODO add your handling code here:
        int SelectedIndex = jComChooser.getSelectedIndex();
        if(selectedSerialPort != null){
            if(selectedSerialPort.isOpen()){
                System.out.println("Disconnecting Port: "+selectedSerialPort.getDescriptivePortName());
                selectedSerialPort.closePort();
                jConnectionStatus.setText("Disconnected");
                jConnectionStatus.setBackground(Color.white);
            }
        }
    }//GEN-LAST:event_jDisconnectButtonMouseClicked
    private DynamicTimeSeriesCollection datasetSens1;
    private SerialPort[] availableSerialPorts;
    private SerialPort selectedSerialPort;
    private ChartPanel jSensorPlot1;
    private JFreeChart jChartSensor1;
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JLabel jBaudLabel;
    private javax.swing.JTextField jBaudRateField;
    private javax.swing.JComboBox<String> jComChooser;
    private javax.swing.JLabel jComLabel;
    private javax.swing.JButton jConnectButton;
    private javax.swing.JLabel jConnectionStatus;
    private javax.swing.JLabel jConnectionStatusLabel;
    private javax.swing.JButton jDisconnectButton;
    private javax.swing.JPanel jGraphFrameSens1;
    private javax.swing.JPanel jGraphFrameSens2;
    private javax.swing.JLabel jSensor1Label;
    private javax.swing.JLabel jSensor2Label;
    private javax.swing.JLabel jSensorTypeLabelSens1;
    private javax.swing.JLabel jSensorTypeLabelSens2;
    private javax.swing.JComboBox<String> jSensorTypeSens1;
    private javax.swing.JComboBox<String> jSensorTypeSens2;
    // End of variables declaration//GEN-END:variables
}
