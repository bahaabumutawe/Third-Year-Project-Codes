% Clear existing serial connections to avoid conflicts
if ~isempty(instrfind)
    fclose(instrfind);
    delete(instrfind);
end

% Setup serial connection - adjust 'COM7' to your Arduino's COM port
comPort = 'COM7';
baudRate = 115200;
s = serial(comPort, 'BaudRate', baudRate);

% Setup callback for data reading
s.BytesAvailableFcnMode = 'terminator';
s.BytesAvailableFcn = @readSerialData;
s.Terminator = 'LF'; % Or set to 'CR/LF' or whatever your Arduino uses

% Open the serial port
fopen(s);

% Declare global variables for storing data and handles to the bar graphs
global stepsData distanceData energyData speedData cadenceData accelerationData restPeriodData
global stepsGraph distanceGraph energyGraph speedGraph cadenceGraph accelerationGraph restPeriodGraph

% Initialize data arrays
stepsData = [];
distanceData = [];
energyData = [];
speedData = [];
cadenceData = [];
accelerationData = [];
restPeriodData = [];

% Initialize the figure with subplots for each metric
modernFigures();

% Callback function to read and plot data
function readSerialData(obj, ~)
    global stepsData distanceData energyData speedData cadenceData accelerationData restPeriodData
    global stepsGraph distanceGraph energyGraph speedGraph cadenceGraph accelerationGraph restPeriodGraph

    dataStr = fscanf(obj); % Reads the data as a string from the serial object
    dataNum = str2double(split(strip(dataStr), ','));
    if numel(dataNum) == 7
        % Update data arrays with the new values
        stepsData(end+1) = dataNum(1);
        distanceData(end+1) = dataNum(2);
        energyData(end+1) = dataNum(3);
        speedData(end+1) = dataNum(4);
        cadenceData(end+1) = dataNum(5);
        accelerationData(end+1) = dataNum(6);
        restPeriodData(end+1) = dataNum(7);
        
        % Update each bar graph
        updateBarGraph(stepsGraph, stepsData);
        updateBarGraph(distanceGraph, distanceData);
        updateBarGraph(energyGraph, energyData);
        updateBarGraph(speedGraph, speedData);
        updateBarGraph(cadenceGraph, cadenceData);
        updateBarGraph(accelerationGraph, accelerationData);
        updateBarGraph(restPeriodGraph, restPeriodData);
    end
end

% Function to initialize figures with modern aesthetics as subplots
function modernFigures()
    global stepsGraph distanceGraph energyGraph speedGraph cadenceGraph accelerationGraph restPeriodGraph

    figure('Name', 'Data Visualization', 'NumberTitle', 'off', 'Color', 'w');
    
    % Initialize each subplot and bar graph
    subplot(3,3,1);
    stepsGraph = bar(0, 'FaceColor', '#0072BD');
    title('Steps Count');
    xlabel('Sample Number');
    ylabel('Steps');
    
    subplot(3,3,2);
    distanceGraph = bar(0, 'FaceColor', '#D95319');
    title('Total Distance Covered');
    xlabel('Sample Number');
    ylabel('Distance (m)');
    
    subplot(3,3,3);
    energyGraph = bar(0, 'FaceColor', '#4DBEEE');
    title('Calories Burnt');
    xlabel('Sample Number');
    ylabel('Calories');
    
    subplot(3,3,4);
    speedGraph = bar(0, 'FaceColor', '#77AC30');
    title('Average Speed');
    xlabel('Sample Number');
    ylabel('Speed (km/h)');
    
    subplot(3,3,5);
    cadenceGraph = bar(0, 'FaceColor', '#A2142F');
    title('Cadence');
    xlabel('Sample Number');
    ylabel('Steps/Minute');
    
    subplot(3,3,6);
    accelerationGraph = bar(0, 'FaceColor', '#EDB120');
    title('Peak Acceleration');
    xlabel('Sample Number');
    ylabel('Acceleration (g)');
    
    subplot(3,3,7);
    restPeriodGraph = bar(0, 'FaceColor', '#FFA07A');
    title('Rest Period Duration');
    xlabel('Sample Number');
    ylabel('Duration (Seconds)');
    
    % Additional configuration for each subplot can be added here
    grid on; box on; axis tight;
end

% Helper function to update a bar graph with new data
function updateBarGraph(graphHandle, data)
    set(graphHandle, 'YData', data); % Update the YData property to reflect new data
    drawnow; % Force MATLAB to redraw the figure
end
