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
rx_SDRu.CenterFrequency = 2402e+06;
rx_SDRu.Gain = 30;
rx_SDRu.SamplesPerFrame = 375000;
rx_SDRu.TransportDataType = 'int8';
rx_SDRu.SampleRate = 4e+06;
rx_SDRu.DecimationFactor = rx_SDRu.MasterClockRate/rx_SDRu.SampleRate;
rx_SDRu.OutputDataType = 'single';
%rx_SDRu.EnableBurstMode = true;
%rx_SDRu.NumFramesInBurst = 1;
disp("Done configuring!");

% Initialize variables
rcvdSignal = complex(zeros(rx_SDRu.FrameLength,1));
sampleRate = 4e+06;

% Filtering Design
%ChannelFilter = BLEFilter(sampleRate);

%
% Setup spectrum viewer
spectrumScope = dsp.SpectrumAnalyzer( ...
    'NumInputPorts',    1,...
    'SampleRate',       sampleRate,...
    'SpectrumType',     'Power density', ...
    'SpectralAverages', 10, ...
    'YLimits',          [-120 0], ...
    'Title',            'Received Baseband BLE Signal Spectrum', ...
    'YLabel',           'Power spectral density');
%

dataSearch1 = ['6' 'D' 'E' 'B' '9' '8' 'E' '8'];
%dataSearch1 = ['2' 'F' '3' '8' '3' 'D' 'F' '6'];

% Threshold for AGC
%detectionThreshold = 0.01;

% Algorithm specific paramters
%accessAddress = [];
%receptionTime = [];
%receptionRssi = [];

%caseCondition = 2;
%AA = false; 
%AAtoSearch = 0;

%occuranceTimer = [];
%previousTime = 0;
%currentTime = 0;

countAppearance = 0;
%p = gcp(); % Get the current parallel pool


while true
    %Receive data from USRP
    %f_data = parfeval(p,@step,1,rx_SDRu);
    %rcvdSignal = fetchOutputs(f_data);
    rcvdSignal = rx_SDRu();  

    %Getting time of received data
    %f_time = parfeval(p,@posixtime,1,datetime('now'));
    %receivdedTime = fetchOutputs(f_time);
    receivdedTime = posixtime(datetime('now'));
    
    % AGC and channel selectivity
    %f_gaussFilt = parfeval(p,@gaussFilteredData,1,rcvdSignal,rx_SDRu.FrameLength,rx_SDRu.SampleRate);
    %rcvFilt = fetchOutputs(f_gaussFilt);
   
    rcvFilt = gaussFilteredData(rcvdSignal,rx_SDRu.FrameLength,sampleRate);
    
    % Filtering Data
    
    %outChanFilt = filter(ChannelFilter, rcvFilt, 1);
    
    % RSSI value
    
    filteredData = squelchedData(rcvFilt, rx_SDRu.SamplesPerFrame);
    
    % Display Signals in Spectrum Analyzer
    spectrumScope(filteredData);
   
    % GMSK demodulator  
    %f_demodData = parfeval(p,@BLErecoveredData,1,filteredData);
    %demodData = fetchOutputs(f_demodData);
    demodData = BLErecoveredData(filteredData);
    pos = strfind(demodData, dataSearch1);
    
    if pos
        countAppearance = countAppearance + 1;
        disp(countAppearance);
        %break;
    end
        %{
        switch caseCondition
            case 0
                % Checking all possible Access Addresses (AA)
                % If a valid AA is found or an AA is given use that instead
                if AA % Is AA provided
                    caseCondition = 2;
                else % If AA is not provided
                    for i = 1:length(pos)
                        % Get AA from data frame
                        addr = extractBetween(demodData,pos(i)+2,pos(i)+7);
                        accessAddress = [accessAddress addr];
                        
                        index = (((pos(i)-1)*4 + 1) - 1)*4 + 1;
                        %Get time of reception
                        t = receivdedTime.*1e3 + index*25e-05;
                        receptionTime = [receptionTime t];

                        %Get RSSI value
                        startIndex = index;
                        endIndex = index + 4*4*8;
                        rssi = mean(10*log10(fftshift(abs(fft(filteredData(startIndex:endIndex))))));
                        receptionRssi = [receptionRssi rssi];
                        
                        % Set here a condition to switch to case 1 when an
                        % AA is found
                    end
                end
            case 1
                % Find out which channels to look for to get the specific
                % AA
            case 2
                %  Got the AA of the connection to follow. Now get the time
                %  of occurance of the connection in a given channel
                if length(pos) > 1
                    for i = 1:length(pos)
                        addr = extractBetween(demodData,pos(i)+2,pos(i)+9);
                        %if addr == AAtoSearch
                            index = (((pos(i)-1)*4 + 1) - 1)*4 + 1;
                            currentTime = receivdedTime.*1e3 + index*25e-05;
                            t = currentTime - previousTime;
                            previousTime = currentTime;
                            occuranceTimer = [occuranceTimer t];
                            disp(t);
                        %end
                    end
                else
                    addr = extractBetween(demodData,pos+0,pos+7);
                        %if addr == AAtoSearch
                            index = (((pos-1)*4 + 1) - 1)*4 + 1;
                            currentTime = receivdedTime.*1e3 + index*25e-05;
                            t = currentTime - previousTime;
                            previousTime = currentTime;
                            occuranceTimer = [occuranceTimer t];
                            % disp(t);
                        %end
                end
        end
        
    end
        %}
end

%}
