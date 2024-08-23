// Requiring module
const express = require('express');
const multer = require('multer');
const bodyParser = require('body-parser');
const path = require('path');

// Creating express object
const app = express();

// middleware for bodyParser
const upload = multer();
app.use(bodyParser.urlencoded({extended: true}));
app.use(bodyParser.json());

// set up the statics folder
app.use(express.static('static'));

// ----------------- Handling Basic GET requests ---------------------
app.get('/',function(req,res){      
    res.sendFile(path.join(__dirname,'static/index.html'));
});

app.get('/fluid',function(req,res){      
    res.sendFile(path.join(__dirname,'static/fluid.html'));
});

app.get('/orbit',function(req,res){      
    res.sendFile(path.join(__dirname,'static/orbit.html'));
});

app.get('/gravsim',function(req,res){      
    res.sendFile(path.join(__dirname,'static/gravsim.html'));
});

app.get('/bounce',function(req,res){      
    res.sendFile(path.join(__dirname,'static/bounce.html'));
});

app.get('/powder',function(req,res){      
    res.sendFile(path.join(__dirname,'static/powder.html'));
});

// ----------------- Handling pythong GET requests -------------------
app.post('/runFluid', upload.none(), (req, res) => {
    const formData = req.body;
    console.log('Form Data:', formData);
    console.log('Form Data:', JSON.stringify(formData));

    const { spawn } = require('child_process');
    const pyProg = spawn('python', ['./CPrograms/Fluid/main.py', JSON.stringify(formData)], {
        // detached: true,
        stdio: 'ignore'
    });

    pyProg.on('exit', (code) => {
        console.log(`Python script exited with code ${code}`);
        // Run additional code here
        if (!res.headersSent) { // Check if the response has already been sent
            res.send('Python script executed successfully.');
        }
        pyProg.unref();
    });

    pyProg.on('error', (err) => {
        console.error('Failed to start subprocess.', err);
        if (!res.headersSent) { // Check if the response has already been sent
            res.status(500).send('Failed to execute Python script.');
        }
    });

    pyProg.unref();
});

app.post('/runGravsim', upload.none(), (req, res) => {
    const formData = req.body;
    console.log('Form Data:', formData);
    console.log('Form Data:', JSON.stringify(formData));

    const { spawn } = require('child_process');
    const pyProg = spawn('python', ['./CPrograms/Gravity/main.py', JSON.stringify(formData)], {
        // detached: true,
        stdio: 'ignore'
    });

    pyProg.on('exit', (code) => {
        console.log(`Python script exited with code ${code}`);
        // Run additional code here
        if (!res.headersSent) { // Check if the response has already been sent
            res.send('Python script executed successfully.');
        }
        pyProg.unref();
    });

    pyProg.on('error', (err) => {
        console.error('Failed to start subprocess.', err);
        if (!res.headersSent) { // Check if the response has already been sent
            res.status(500).send('Failed to execute Python script.');
        }
    });

    pyProg.unref();
});

app.post('/runBounce', upload.none(), (req, res) => {
    const formData = req.body;
    console.log('Form Data:', formData);
    console.log('Form Data:', JSON.stringify(formData));

    const { spawn } = require('child_process');
    const pyProg = spawn('python', ['./CPrograms/Bounce/main.py', JSON.stringify(formData)], {
        // detached: true,
        stdio: 'ignore'
    });

    pyProg.on('exit', (code) => {
        console.log(`Python script exited with code ${code}`);
        // Run additional code here
        if (!res.headersSent) { // Check if the response has already been sent
            res.send('Python script executed successfully.');
        }
        pyProg.unref();
    });

    pyProg.on('error', (err) => {
        console.error('Failed to start subprocess.', err);
        if (!res.headersSent) { // Check if the response has already been sent
            res.status(500).send('Failed to execute Python script.');
        }
    });

    pyProg.unref();
});

app.post('/runOrbit', upload.none(), (req, res) => {
    const formData = req.body;
    console.log('Form Data:', formData);
    console.log('Form Data:', JSON.stringify(formData));

    const { spawn } = require('child_process');
    const pyProg = spawn('python', ['./CPrograms/Orbit/main.py', JSON.stringify(formData)], {
        // detached: true,
        stdio: 'ignore'
    });

    pyProg.on('exit', (code) => {
        console.log(`Python script exited with code ${code}`);
        // Run additional code here
        if (!res.headersSent) { // Check if the response has already been sent
            res.send('Python script executed successfully.');
        }
        pyProg.unref();
    });

    pyProg.on('error', (err) => {
        console.error('Failed to start subprocess.', err);
        if (!res.headersSent) { // Check if the response has already been sent
            res.status(500).send('Failed to execute Python script.');
        }
    });

    pyProg.unref();
});


// ----------------- port and server starting ------------------
// Port Number
const PORT = process.env.PORT || 5000;

// Server Setup
app.listen(PORT,console.log(
  `Server started on port ${PORT}`));