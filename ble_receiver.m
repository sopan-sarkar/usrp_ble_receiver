clc;
clearvars;
% Check USRP connection and get platform and address
connectedRadios = findsdru;
if strncmp(connectedRadios(1).Status, 'Success', 7)
  switch connectedRadios(1).Platform
    case {'B200','B210'}
      address = connectedRadios(1).SerialNum;
      platform = connectedRadios(1).Platform;
    case {'N200/N210/USRP2'}
      address = connectedRadios(1).IPAddress;
      platform = 'N200/N210/USRP2';
    case {'X300','X310'}
      address = connectedRadios(1).IPAddress;
      platform = connectedRadios(1).Platform;
  end
else
  address = '192.168.10.2';
  platform = 'N200/N210/USRP2';
end
% End checking connection

%Receiver parameter structure
rx_SDRu = comm.SDRuReceiver('Platform',platform,'IPAddress',address);
rx_SDRu.CenterFrequency = 2404e+06;
rx_SDRu.Gain = 30;
rx_SDRu.SamplesPerFrame = 250000;
rx_SDRu.TransportDataType = 'int8';
rx_SDRu.SampleRate = 2e+06;
rx_SDRu.DecimationFactor = rx_SDRu.MasterClockRate/rx_SDRu.SampleRate;
rx_SDRu.OutputDataType = 'single';
rx_SDRu.EnableBurstMode = true;
rx_SDRu.NumFramesInBurst = 8;
disp("Done configuring!");
%{
% Check actual radio settings
rx_log = dsp.SignalSink;
    for counter = 1:20
      data = rx_SDRu();
      rx_log(data);
    end
info(rx_SDRu)
%rx_SDRu
%}

