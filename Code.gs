function doPost(e) {
  const d = JSON.parse(e.postData.contents);
  const sh = SpreadsheetApp.getActiveSpreadsheet().getSheets()[0];
 
  if (sh.getLastRow() === 0) {
    sh.appendRow(["วัน เวลา", "ค่าความมั่นใจ ชื่อบอร์ด", "ตัวที่ส่ง"]);
  }
 
  // "2025-10-04 14:26:10" -> Date (เวลาไทย)
  let when = d.datetime;
  const m = /^(\d{4})-(\d{2})-(\d{2}) (\d{2}):(\d{2}):(\d{2})$/.exec(d.datetime || "");
  if (m) when = new Date(`${m[1]}-${m[2]}-${m[3]}T${m[4]}:${m[5]}:${m[6]}+07:00`);
 
  sh.appendRow([when, d.message, d.sender]);
  const row = sh.getLastRow();
  sh.getRange(row, 1).setNumberFormat("d/M/yyyy, HH:mm:ss");
 
  return ContentService.createTextOutput("OK");
}
 
