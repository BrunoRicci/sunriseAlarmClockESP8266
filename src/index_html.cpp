#include <avr/pgmspace.h>

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.5">
  <!-- <link rel="stylesheet" href="src/style.css"> -->
</head>

<body>
  <main>
    <div class="clock" id="clock">
      00:00:00
    </div>
    <div class="alarms">
      <div class="date-input-container">
        <input id="mondayEnabled" for="mondayTime" type="checkbox"><label>Lunes</label><input type="time"
          id="mondayTime" value="00:00">
      </div>
      <div class="date-input-container">
        <input id="tuesdayEnabled" for="tuesdayTime" type="checkbox"><label>Martes</label><input type="time"
          id="tuesdayTime" value="00:00">
      </div>
      <div class="date-input-container">
        <input id="wednesdayEnabled" for="wednesdayTime" type="checkbox"><label>Miércoles</label><input type="time"
          id="wednesdayTime" value="00:00">
      </div>
      <div class="date-input-container">
        <input id="thursdayEnabled" for="thursdayTime" type="checkbox"><label>Jueves</label><input type="time"
          id="thursdayTime" value="00:00">
      </div>
      <div class="date-input-container">
        <input id="fridayEnabled" for="fridayTime" type="checkbox"><label>Viernes</label><input type="time"
          id="fridayTime" value="00:00">
      </div>
      <div class="date-input-container">
        <input id="saturdayEnabled" for="saturdayTime" type="checkbox"><label>Sábado</label><input type="time"
          id="saturdayTime" value="00:00">
      </div>
      <div class="date-input-container">
        <input id="sundayEnabled" for="sundayTime" type="checkbox"><label>Domingo</label><input type="time"
          id="sundayTime" value="00:00">
      </div>
      <input type="checkbox" value="disableAll" id="disableAll"><span style="font-size:15px;">Desactivar</span>
    </div>
  </main>
  <div class="lights">
    <div class="light-container">
      <div style="display: flex;align-items: center;">
        <svg height="40px" width="40px" fill="#000000" viewBox="0 0 32 32" version="1.1"
          xmlns="http://www.w3.org/2000/svg">
          <g id="SVGRepo_bgCarrier" stroke-width="0"></g>
          <g id="SVGRepo_tracerCarrier" stroke-linecap="round" stroke-linejoin="round"></g>
          <g id="SVGRepo_iconCarrier">
            <title>brightness-low</title>
            <path
              d="M16 11.25c-2.623 0-4.75 2.127-4.75 4.75s2.127 4.75 4.75 4.75c2.623 0 4.75-2.127 4.75-4.75v0c-0.003-2.622-2.128-4.747-4.75-4.75h-0zM16 19.25c-1.795 0-3.25-1.455-3.25-3.25s1.455-3.25 3.25-3.25c1.795 0 3.25 1.455 3.25 3.25v0c-0.002 1.794-1.456 3.248-3.25 3.25h-0zM16 8.75c0.414-0 0.75-0.336 0.75-0.75v0-1c0-0.414-0.336-0.75-0.75-0.75s-0.75 0.336-0.75 0.75v0 1c0 0.414 0.336 0.75 0.75 0.75v0zM16 23.25c-0.414 0-0.75 0.336-0.75 0.75v0 1c0 0.414 0.336 0.75 0.75 0.75s0.75-0.336 0.75-0.75v0-1c-0-0.414-0.336-0.75-0.75-0.75v0zM8 15.25h-1c-0.414 0-0.75 0.336-0.75 0.75s0.336 0.75 0.75 0.75v0h1c0.414 0 0.75-0.336 0.75-0.75s-0.336-0.75-0.75-0.75v0zM25 15.25h-1c-0.414 0-0.75 0.336-0.75 0.75s0.336 0.75 0.75 0.75v0h1c0.414 0 0.75-0.336 0.75-0.75s-0.336-0.75-0.75-0.75v0zM10.166 9.105c-0.135-0.131-0.319-0.212-0.523-0.212-0.414 0-0.75 0.336-0.75 0.75 0 0.203 0.081 0.388 0.213 0.523l0.707 0.707c0.135 0.131 0.319 0.212 0.522 0.212 0.414 0 0.75-0.336 0.75-0.75 0-0.203-0.081-0.387-0.212-0.522l0 0zM22.188 21.125c-0.136-0.136-0.324-0.22-0.531-0.22-0.415 0-0.751 0.336-0.751 0.751 0 0.207 0.084 0.395 0.22 0.531l0.707 0.707c0.136 0.136 0.324 0.22 0.531 0.22 0.415 0 0.751-0.336 0.751-0.751 0-0.207-0.084-0.395-0.22-0.531v0zM9.813 21.125l-0.707 0.707c-0.135 0.136-0.218 0.323-0.218 0.529 0 0.415 0.336 0.751 0.751 0.751 0.206 0 0.393-0.083 0.528-0.218l0.707-0.707c0.136-0.136 0.22-0.324 0.22-0.531 0-0.415-0.336-0.751-0.751-0.751-0.207 0-0.395 0.084-0.53 0.219v0zM21.832 9.105l-0.707 0.707c-0.136 0.136-0.219 0.323-0.219 0.53 0 0.415 0.336 0.751 0.751 0.751 0.208 0 0.395-0.084 0.531-0.22v0l0.707-0.707c0.134-0.136 0.218-0.322 0.218-0.528 0-0.415-0.336-0.751-0.751-0.751-0.207 0-0.394 0.083-0.529 0.219l0-0z">
            </path>
          </g>
        </svg>
        <input type="range" min="0" max="100" id="brightness">
      </div>
      <input disabled type="color" value="#bb33bb" id="rgb">
    </div>
    <div>
      <span>Decay:</span>
      <input type="number" min="0" step="5" id="decayTime">
      <span>seg</span>
      
    </div>
    <div style="margin-top:30px;">
      <input type="checkbox" value="lampOn" id="pepito" for="lampOn"><span style="font-size:15px;">Luz</span>
      <input type="time"
          id="lampOn" value="00:00" style="display: none;">
    </div>
  </div>

</body>
<script>
  function arrToTime(arrayDates) {
    return `${("0" + arrayDates[0]).slice(-2)}:${("0" + arrayDates[1]).slice(-2)}`;
  }

  function timeToArr(time, enable) {
    let arr = time.split(':');
    let hours = parseInt(arr[0]);
    let minutes = parseInt(arr[1]);
    return [hours, minutes, enable ? 1:0];
  }

  async function getVars() {
    const response = await fetch("/getvars");
    return await response.json();
  }

  async function sendVars(v) {
    console.log('Send vars:', v)
    return await fetch("/config/set", {
      method: 'POST',
      cache: 'no-cache',
      headers: {
        "Content-Type": "application/json"
      },
      body: JSON.stringify(v)
    });
  }

  function toggleDaysCheckboxes() {
    document.querySelectorAll('.date-input-container>input:is([type="checkbox"],[type="time"])').forEach((x) => {
      x.toggleAttribute('disabled');
    });
  }

  function checkTime(i) {
    return i < 10 ? "0" + i : i;  // add zero in front of numbers < 10
  }

  var time;
  window.addEventListener('load', async () => {
    console.log('doc Loaded');
    let myVars = await getVars();
    console.log('Fetch /getvars:', myVars);

    document.getElementById('mondayTime').value = arrToTime(myVars.mondayTime);
    document.getElementById('mondayEnabled').checked = myVars.mondayTime[2];
    document.getElementById('tuesdayTime').value = arrToTime(myVars.tuesdayTime);
    document.getElementById('tuesdayEnabled').checked = myVars.tuesdayTime[2];
    document.getElementById('wednesdayTime').value = arrToTime(myVars.wednesdayTime);
    document.getElementById('wednesdayEnabled').checked = myVars.wednesdayTime[2];
    document.getElementById('thursdayTime').value = arrToTime(myVars.thursdayTime);
    document.getElementById('thursdayEnabled').checked = myVars.thursdayTime[2];
    document.getElementById('fridayTime').value = arrToTime(myVars.fridayTime);
    document.getElementById('fridayEnabled').checked = myVars.fridayTime[2];
    document.getElementById('saturdayTime').value = arrToTime(myVars.saturdayTime);
    document.getElementById('saturdayEnabled').checked = myVars.saturdayTime[2];
    document.getElementById('sundayTime').value = arrToTime(myVars.sundayTime);
    document.getElementById('sundayEnabled').checked = myVars.sundayTime[2];

    document.getElementById('brightness').value = myVars.brightness;
    document.getElementById('decayTime').value = myVars.decayTime;
    // document.getElementById('rgb').value = myVars.rgb;
    document.getElementById('disableAll').checked = myVars.disableAll;
    if(myVars.disableAll) toggleDaysCheckboxes();
    document.getElementById('lampOn').value = myVars.lampOn;
    
    time = new Date(myVars.time);
    console.log('received time:', myVars.time);
    console.log('date:', time);
    setInterval(()=>{
      let clock = document.getElementById('clock');
      time.setSeconds(time.getSeconds()+1);
      clock.innerHTML = `${checkTime(time.getHours())}:${checkTime(time.getMinutes())}:${checkTime(time.getSeconds())}`;
    },1000);
  });


  document.getElementById('disableAll').addEventListener('change', async (e) => {
    toggleDaysCheckboxes();
    let val = {};
    val[`${e.target.id}`] = e.target.checked;
    await sendVars(val);
  });

  document.querySelectorAll('input').forEach((x) => {
    x.addEventListener('change', (e) => {
      let element = e.target;
      let val, valName;
      if (element.id == 'disableAll') return;
      console.log('input changed', element);
      valName = element.id;
      if (element.getAttribute('type') == 'time') {
        let chbx = document.querySelector(`input[for=\"${element.id}\"]`);
        val = timeToArr(element.value, chbx.checked);
      } else if (element.getAttribute('type') == 'checkbox') {
        // val = element.checked;
        let t = document.getElementById(`${element.getAttribute('for')}`);
        val = timeToArr(t.value, element.checked);
        valName = t.id;
      } else if (element.getAttribute('type') == 'number') {
        val = parseInt(element.value);
      } else {
        val = element.value
      }

      if(element.id =='pepito')val = element.checked; 
      var json = {}
      json[`${valName}`] = val;
      sendVars(json);
    })
  })


</script>
<style>
  body {
    background-color: white;
    font-family: 'Roboto', 'Helvetica';
    display: flex;
    flex-direction: column;
    align-items: center;
    font-size: 25px;
  }

  main {
    display: flex;
    flex-direction: column;
  }

  label {
    font-weight: bold;
  }

  .clock{
    width: 100%;
    padding: 5px;
    margin: 5px;
    border-bottom: 2px solid gray;
    display: flex;
    justify-content: center;
  }

  .alarms {
    margin-bottom: 10px;
  }

  input[type="number"] {
    width: 60px;
    margin-top: 10px;
  }


  .lights {
    display: flex;
    flex-direction: column;

    width: 230px;
    margin-bottom: 10px;
    justify-content: center;
  }

  .light-container {
    width: 100%;
    display: flex;
    justify-content: space-between;
    align-items: center;
  }

  #disableAll {
    margin-left: 15px;
  }

  .date-input-container {
    display: flex;
    align-content: center;
    margin-top: 10px;
    margin-bottom: 10px;
  }

  .date-input-container>label {
    width: 60%;
  }

  .date-input-container>input[type="checkbox"] {
    margin-right: 15px;
    margin-left: 15px;
    height: 20px;
    width: 20px;
  }

  .date-input-container>input[type="time"] {
    margin-right: 15px;
    margin-left: 15px;
    height: 20px;
    width: 35%;
  }
</style>

</html>
)rawliteral";