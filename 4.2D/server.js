const express = require("express");
const cors = require("cors");
require("dotenv").config();

const app = express();
const PORT = 3000;

app.use(cors());
app.use(express.json());
app.use(express.static(__dirname));

const BLYNK_TOKEN = process.env.BLYNK_AUTH_TOKEN;

const roomPins = {
    "living room": "V0",
    "bathroom": "V1",
    "closet": "V2"
};

app.post("/control-light", async (req, res) => {
    const { room, state } = req.body;

    const pin = roomPins[room];

    if (!pin) {
        return res.status(400).json({
            success: false,
            message: "Invalid room"
        });
    }

    const value = state ? 1 : 0;

    try {
        const url =
            `https://blynk.cloud/external/api/update?token=${BLYNK_TOKEN}&${pin}=${value}`;

        const response = await fetch(url);

        if (!response.ok) {
            throw new Error("Blynk request failed");
        }

        res.json({
            success: true,
            room: room,
            state: state
        });

    } catch (error) {
        console.error("Blynk error:", error);

        res.status(500).json({
            success: false,
            message: "Could not update Blynk"
        });
    }
});

app.get("/", (req, res) => {
    res.sendFile(__dirname + "/index.html");
});

app.listen(PORT, () => {
    console.log(`Server running at http://localhost:${PORT}`);
});