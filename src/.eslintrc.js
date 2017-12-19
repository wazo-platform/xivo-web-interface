module.exports = {
  "env": {
    "browser": true,
    "es6": true,
    "node": true
  },
  "parser": "babel-eslint",
  "extends": "eslint:recommended",
  "parserOptions": {
    "sourceType": "module"
  },
  "globals": {
    "Cti": true,
    "Callback": true,
    "Membership": true,
    "xc_webrtc": true,
    "angular": true,
    "DirectoryDisplay": true
  },
  "rules": {
    "struct": 0,
    "indent": [
      "error",
      2
    ],
    "linebreak-style": [
      "error",
      "unix"
    ],
    "quotes": 0,
    "semi": [
      "error",
      "always"
    ],
    "no-unused-vars": [
      "warn"
    ]
  },
  "plugins": ["babel"]
};
