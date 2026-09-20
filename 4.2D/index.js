document.getElementById("livingRoom").addEventListener("change", function () {
    controlLight("living room", this.checked);
});

document.getElementById("bathroom").addEventListener("change", function () {
    controlLight("bathroom", this.checked);
});

document.getElementById("closet").addEventListener("change", function () {
    controlLight("closet", this.checked);
});

async function controlLight(room, state) {
    console.log("Sending:", room, state);

    try {
        const response = await fetch("/control-light", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify({
                room: room,
                state: state
            })
        });

        const result = await response.json();

        console.log("Backend response:", result);

        if (!response.ok) {
            console.error("Failed to control light");
        }

    } catch (error) {
        console.error("Connection error:", error);
    }
}