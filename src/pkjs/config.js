module.exports = [
  { 
    "type": "heading", 
    "defaultValue": "tiny3words configuration" 
  }, 
  { 
    "type": "text", 
    "defaultValue": "Here you can configure your autorefresh" 
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Settings"
      },
      {
        "type": "toggle",
        "messageKey": "enableRefresh",
        "label": "Turn on autorefresh?"
      },
      {
        "type": "slider",
        "messageKey": "interval",
        "defaultValue": 0.25,
        "label": "Autorefresh interval",
        "description": "Time between automatic refreshes (minutes)",
        "min": 0,
        "max": 5,
        "step": 0.25
      }
    ]
  },
  {
  "type": "submit",
  "defaultValue": "Save"
  }
];