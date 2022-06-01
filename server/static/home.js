const CHART_COLORS = {
  red: 'rgb(255, 99, 132)',
  orange: 'rgb(255, 159, 64)',
  yellow: 'rgb(255, 205, 86)',
  green: 'rgb(75, 192, 192)',
  blue: 'rgb(54, 162, 235)',
  purple: 'rgb(153, 102, 255)',
  grey: 'rgb(201, 203, 207)'
};
 
var gyroRawData = window.gyroData["gyro_data"];
var weatherRawData = window.weatherData["weather_data"];

var weatherTimeLabels = [];
weatherRawData.forEach(element => weatherTimeLabels.push(element["time"]));
console.log(weatherTimeLabels);

var tempData = [];
weatherRawData.forEach(element => tempData.push(element["temp"]));

var humidData = [];
weatherRawData.forEach(element => humidData.push(element["humidity"]));
const tempChart = new Chart(document.getElementById("tempChart"), {
	type: "line",
	data: {
		labels: weatherTimeLabels,
		datasets: [
			{
				label: "Temperature (Celsius)",
				data: tempData,
				borderColor: CHART_COLORS.red,
				fill: false	
			}
		]


	},
	options: {
		responsive: true,
		plugins: {
			legend: {position: "top"},
			title: {display: true, text: "Temperature"}
		}
	}
});

const weatherChart = new Chart(document.getElementById("humidityChart"), {
	type: "line",
	data: {
		labels: weatherTimeLabels,
		datasets: [
			{
				label: "Humidity",
				data: humidData,
				borderColor: CHART_COLORS.blue,
				fill: false
			}
		]


	},
	options: {
		responsive: true,
		plugins: {
			legend: {position: "top"},
			title: {display: true, text: "Humidity Chart"}
		}
	}
});

var gyroTimeLabels = [];
gyroRawData.forEach(element => gyroTimeLabels.push(element["time"]));

var gyroX = [];
gyroRawData.forEach(element => gyroX.push(element["gyroX"]));

var gyroY = [];
gyroRawData.forEach(element => gyroY.push(element["gyroY"]));

var gyroZ = [];
gyroRawData.forEach(element => gyroZ.push(element["gyroZ"]));

const gyroChart = new Chart(document.getElementById("gyroChart"), {
	type: "line",
	data: {
		labels: gyroTimeLabels,
		datasets: [
			{
				label: "Gyro X",
				data: gyroX,
				borderColor: CHART_COLORS.red,
				fill: false
			},
			{
				label: "Gyro Y",
				data: gyroY,
				borderColor: CHART_COLORS.blue,
				fill: false
			},
			{
				label: "Gyro Z",
				data: gyroZ,
				borderColor: CHART_COLORS.green,
				fill: false
			}
		]


	},
	options: {
		responsive: true,
		plugins: {
			legend: {position: "top"},
			title: {display: true, text: "Gyroscope Chart"}
		}
	}
});

var accelTimeLabels = [];
gyroRawData.forEach(element => accelTimeLabels.push(element["time"]));

var accelX = [];
gyroRawData.forEach(element => accelX.push(element["accelX"]));

var accelY = [];
gyroRawData.forEach(element => accelY.push(element["accelY"]));

var accelZ = [];
gyroRawData.forEach(element => accelZ.push(element["accelZ"]));

const accelChart = new Chart(document.getElementById("accelChart"), {
	type: "line",
	data: {
		labels: accelTimeLabels,
		datasets: [
			{
				label: "Accel X",
				data: accelX,
				borderColor: CHART_COLORS.red,
				fill: false
			},
			{
				label: "Accel Y",
				data: accelY,
				borderColor: CHART_COLORS.blue,
				fill: false
			},
			{
				label: "Accel Z",
				data: accelZ,
				borderColor: CHART_COLORS.green,
				fill: false
			}
		]


	},
	options: {
		responsive: true,
		plugins: {
			legend: {position: "top"},
			title: {display: true, text: "Gyroscope Chart"}
		}
	}
});


var gyroRMSData = [];
gyroRawData.forEach(element => gyroRMSData.push(element["gyroRMS"]));
const gyroRMSChart = new Chart(document.getElementById("gyroRMSChart"), {
	type: "line",
	data: {
		labels: gyroTimeLabels,
		datasets: [
			{
				label: "Gyro RMS (Root Mean Square)",
				data: gyroRMSData,
				borderColor: CHART_COLORS.red,
				fill: false	
			}
		]


	},
	options: {
		responsive: true,
		plugins: {
			legend: {position: "top"},
			title: {display: true, text: "Gyro RMS"}
		}
	}
});

var accelRMSData = [];
gyroRawData.forEach(element => accelRMSData.push(element["accelRMS"]));
const accelRMSChart = new Chart(document.getElementById("accelRMSChart"), {
	type: "line",
	data: {
		labels: gyroTimeLabels,
		datasets: [
			{
				label: "Accel RMS (Root Mean Square)",
				data: accelRMSData,
				borderColor: CHART_COLORS.red,
				fill: false	
			}
		]


	},
	options: {
		responsive: true,
		plugins: {
			legend: {position: "top"},
			title: {display: true, text: "Accel RMS"}
		}
	}
});
