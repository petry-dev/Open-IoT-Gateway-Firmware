/* * PROJECT: Open IoT Gateway Web Config
 * DESCRIPTION: Handles UI interactions, translations (i18n), and asynchronous 
 * communication with the ESP32 backend.
 */

// 1. Translation Dictionary
const translations = {
    en: {
        success_title: "Configuration Saved!",
        success_msg: "Device connected successfully. Rebooting...",
        app_title: "Open IoT Gateway",
        app_subtitle: "Industrial Connectivity",
        step1_title: "General Settings",
        lbl_machine_type: "Machine Type",
        opt_loading: "Loading...",
        opt_select: "Select Machine...",
        lbl_cnc_ip: "CNC IP Address",
        hint_cnc: "Required for MTConnect protocol",
        sec_wifi: "Wi-Fi Connection",
        lbl_network: "Select Network",
        lbl_password: "Password",
        adv_title: "Advanced Configuration",
        adv_desc: "Broker MQTT, User Auth & Custom Settings",
        lbl_broker: "MQTT Broker IP",
        lbl_user: "MQTT User",
        lbl_mqtt_pass: "MQTT Password",
        hint_topic: "Topic is auto-generated:",
        btn_next: "Next Step",
        step2_title: "Sensors Map",
        step2_desc: "Select the physical inputs connected to the machine:",
        btn_back: "Back",
        step3_title: "Validation & Save",
        sum_machine: "Machine:",
        sum_wifi: "Wi-Fi SSID:",
        sum_cnc: "CNC IP:",
        st_wifi: "Wi-Fi Connection",
        st_mqtt: "MQTT Broker",
        st_wait: "Waiting",
        st_ok: "OK",
        st_fail: "Fail",
        btn_test: "Test Connection",
        btn_save: "Save Configuration",
        msg_test: "Testing...",
        msg_wait: "Please wait..."
    },
    pt: {
        success_title: "Configuração Salva!",
        success_msg: "Conexões realizadas com sucesso. Reiniciando...",
        app_title: "Open IoT Gateway",
        app_subtitle: "Conectividade Industrial",
        step1_title: "Configurações Gerais",
        lbl_machine_type: "Tipo de Máquina",
        opt_loading: "Carregando...",
        opt_select: "Selecione a Máquina...",
        lbl_cnc_ip: "Endereço IP da CNC",
        hint_cnc: "Necessário para protocolo MTConnect",
        sec_wifi: "Conexão Wi-Fi",
        lbl_network: "Selecione a Rede",
        lbl_password: "Senha",
        adv_title: "Configuração Avançada",
        adv_desc: "Broker MQTT, Autenticação e Ajustes",
        lbl_broker: "IP do Broker MQTT",
        lbl_user: "Usuário MQTT",
        lbl_mqtt_pass: "Senha MQTT",
        hint_topic: "Tópico gerado automaticamente:",
        btn_next: "Próximo",
        step2_title: "Mapa de Sensores",
        step2_desc: "Selecione as entradas físicas conectadas à máquina:",
        btn_back: "Voltar",
        step3_title: "Validação e Salvar",
        sum_machine: "Máquina:",
        sum_wifi: "Rede Wi-Fi:",
        sum_cnc: "IP CNC:",
        st_wifi: "Conexão Wi-Fi",
        st_mqtt: "Broker MQTT",
        st_wait: "Aguardando",
        st_ok: "Sucesso",
        st_fail: "Falha",
        btn_test: "Testar Conexão",
        btn_save: "Salvar Configuração",
        msg_test: "Testando...",
        msg_wait: "Por favor aguarde..."
    },
    es: {
        success_title: "¡Configuración Guardada!",
        success_msg: "Conexiones exitosas. Reiniciando...",
        app_title: "Open IoT Gateway",
        app_subtitle: "Conectividad Industrial",
        step1_title: "Configuración General",
        lbl_machine_type: "Tipo de Máquina",
        opt_loading: "Cargando...",
        opt_select: "Seleccione Máquina...",
        lbl_cnc_ip: "Dirección IP del CNC",
        hint_cnc: "Requerido para protocolo MTConnect",
        sec_wifi: "Conexión Wi-Fi",
        lbl_network: "Seleccionar Red",
        lbl_password: "Contraseña",
        adv_title: "Configuración Avanzada",
        adv_desc: "Broker MQTT, Autenticación y Ajustes",
        lbl_broker: "IP del Broker MQTT",
        lbl_user: "Usuario MQTT",
        lbl_mqtt_pass: "Contraseña MQTT",
        hint_topic: "Tópico generado automáticamente:",
        btn_next: "Siguiente",
        step2_title: "Mapa de Sensores",
        step2_desc: "Seleccione las entradas físicas conectadas a la máquina:",
        btn_back: "Volver",
        step3_title: "Validación y Guardar",
        sum_machine: "Máquina:",
        sum_wifi: "Red Wi-Fi:",
        sum_cnc: "IP CNC:",
        st_wifi: "Conexión Wi-Fi",
        st_mqtt: "Broker MQTT",
        st_wait: "Esperando",
        st_ok: "Éxito",
        st_fail: "Fallo",
        btn_test: "Probar Conexión",
        btn_save: "Guardar Configuración",
        msg_test: "Probando...",
        msg_wait: "Por favor espere..."
    }
};

let currentLang = 'en';

// 2. Initialization & Lifecycle
window.onload = function () {
    // Load Theme from LocalStorage (defaults to Dark)
    const t = localStorage.getItem('theme') || 'dark';
    document.documentElement.setAttribute('data-theme', t);
    updateThemeIcon(t);

    // Load Language (Robust Fallback)
    let l = localStorage.getItem('lang');
    if (!l || !translations[l]) {
        l = 'en';
    }

    // Apply language and load dynamic data
    setLanguage(l);

    // Remove loading screen overlay
    document.body.classList.remove('loading');

    // Trigger initial Wi-Fi Scan
    atualizarRedes();
};

function toggleTheme() {
    const current = document.documentElement.getAttribute('data-theme');
    const target = current === 'light' ? 'dark' : 'light';

    document.documentElement.setAttribute('data-theme', target);
    localStorage.setItem('theme', target);
    updateThemeIcon(target);
}

function updateThemeIcon(theme) {
    const icon = document.getElementById('theme-icon');
    if (theme === 'light') {
        icon.className = 'las la-sun';
    } else {
        icon.className = 'las la-moon';
    }
}

function setLanguage(lang) {
    // Fallback if language key doesn't exist
    if (!translations[lang]) lang = 'en';
    currentLang = lang;
    localStorage.setItem('lang', lang);

    // Update Flag Buttons
    const btns = document.querySelectorAll('.lang-switch button');
    btns.forEach(b => b.classList.remove('active'));

    if (lang === 'pt') btns[0].classList.add('active');
    else if (lang === 'en') btns[1].classList.add('active');
    else if (lang === 'es') btns[2].classList.add('active');

    // Apply translations to data-i18n elements
    document.querySelectorAll('[data-i18n]').forEach(el => {
        const key = el.getAttribute('data-i18n');
        if (translations[lang][key]) {
            el.textContent = translations[lang][key];
        }
    });

    // Update Dynamic Texts (Selects)
    const sel = document.getElementById('machineType');
    if (sel && sel.options[0].disabled) {
        sel.options[0].text = translations[lang].opt_loading;
    }

    loadData();
}

/* * NETWORK & VALIDATION LOGIC 
 * Sends credentials to ESP32 to verify connectivity before saving.
 */
function performTests() {
    // Gather Data
    const ssid = document.getElementById('network-select').value;
    const pass = document.getElementById('password').value;
    const broker = document.querySelector('input[name="mqttServer"]').value;
    const mqttUser = document.querySelector('input[name="mqttUser"]').value;
    const mqttPass = document.getElementById('mqttPass').value;

    if (!ssid || !broker) {
        alert("Error: Wi-Fi SSID and MQTT Broker IP are required for testing.");
        return;
    }

    // UI Feedback
    const btn = document.querySelector('.btn-test');
    btn.disabled = true;
    const msgBox = document.getElementById('test-msg');
    msgBox.textContent = translations[currentLang].msg_test || "Testing Connection...";
    msgBox.style.color = "var(--text-color)";

    // Reset Badges
    document.querySelector('#status-wifi .badge').style.background = "#444";
    document.querySelector('#status-wifi .badge').textContent = "...";
    document.querySelector('#status-mqtt .badge').style.background = "#444";
    document.querySelector('#status-mqtt .badge').textContent = "...";

    // Build Query String manually
    const params = new URLSearchParams();
    params.append('ssid', ssid);
    params.append('password', pass);
    params.append('mqttServer', broker);
    params.append('mqttUser', mqttUser);
    params.append('mqttPassword', mqttPass);

    // Send Request to Backend
    fetch('/validate?' + params.toString())
        .then(r => r.json())
        .then(res => {
            // Update Wi-Fi Status
            const wBad = document.querySelector('#status-wifi .badge');
            wBad.textContent = res.wifi ? "OK" : "FAIL";
            wBad.style.background = res.wifi ? "#04d361" : "#f75a68";

            // Update MQTT Status
            const mBad = document.querySelector('#status-mqtt .badge');
            mBad.textContent = res.mqtt ? "OK" : "FAIL";
            mBad.style.background = res.mqtt ? "#04d361" : "#f75a68";

            msgBox.textContent = res.message;
            msgBox.style.color = (res.wifi && res.mqtt) ? "#04d361" : "#f75a68";

            // Enable Save Button ONLY if both checks pass
            if (res.wifi && res.mqtt) {
                const b = document.getElementById('btn-save');
                b.disabled = false;
                b.style.opacity = "1";
                b.style.cursor = "pointer";
            } else {
                const b = document.getElementById('btn-save');
                b.disabled = true;
                b.style.opacity = "0.5";
                b.style.cursor = "not-allowed";
            }
        })
        .catch(err => {
            msgBox.textContent = "Error communicating with Gateway.";
            console.error(err);
        })
        .finally(() => btn.disabled = false);
}

/* DATA LOADING & RENDERING */
function loadData() {
    const data = window.MACHINES_DATA;
    if (!data) return;

    const sel = document.getElementById('machineType');
    const currentSelection = sel.value;

    sel.innerHTML = `<option value="" disabled selected>${translations[currentLang].opt_select}</option>`;

    // Populate Machine List
    (data.machines || data).forEach(m => {
        let opt = document.createElement('option');
        opt.value = m.id;

        // Localized name handling
        if (typeof m.name === 'object') {
            opt.textContent = m.name[currentLang] || m.name['en'] || Object.values(m.name)[0];
        } else {
            opt.textContent = m.name;
        }

        sel.appendChild(opt);
    });

    // Restore saved selection
    if (window.SAVED.type && !currentSelection) {
        sel.value = window.SAVED.type;
    } else if (currentSelection) {
        sel.value = currentSelection;
    }

    const container = document.getElementById('sensors-container');
    container.innerHTML = '';

    // Generate Sensors List
    (data.sensors || []).forEach(s => {
        let lbl = document.createElement('label');
        lbl.className = "checkbox-wrapper sensor-item";

        let sName = s.name;
        if (typeof s.name === 'object') {
            sName = s.name[currentLang] || s.name['en'] || Object.values(s.name)[0];
        }

        // Create Switch UI
        lbl.innerHTML = `
            <div class="switch-container">
                <input type="checkbox" value="${s.id}" class="sensor-chk switch-input">
                <span class="slider"></span>
            </div>
            <span class="sensor-label">${sName}</span>
        `;
        container.appendChild(lbl);
    });
}

function goToStep(step) {
    if (step === 2) {
        // Validate Step 1
        const ssid = document.getElementById('network-select').value;
        if (!ssid) {
            alert("Wi-Fi is required!");
            return;
        }
    }

    if (step === 3) {
        updateSummary();
    }

    document.querySelectorAll('.step-card').forEach(c => c.classList.add('collapsed'));
    document.getElementById('step' + step).classList.remove('collapsed');
}

function updateSummary() {
    const data = window.MACHINES_DATA;

    // 1. Get Selected Machine
    const machId = document.getElementById('machineType').value;
    const machObj = data.machines.find(m => m.id === machId);
    let machName = machObj ? (machObj.name[currentLang] || machObj.name['en'] || machObj.name) : '-';
    document.getElementById('sum-machine').textContent = machName;

    // 2. Get Wi-Fi & MQTT Data
    document.getElementById('sum-ssid').textContent = document.getElementById('network-select').value || '-';
    document.getElementById('sum-broker').textContent = document.querySelector('input[name="mqttServer"]').value || '-';
    document.getElementById('sum-user').textContent = document.querySelector('input[name="mqttUser"]').value || '-';

    // 3. Sensor List (Badges)
    const checkedSensors = Array.from(document.querySelectorAll('.sensor-chk:checked')).map(c => c.value);
    const sensorContainer = document.getElementById('sum-sensors');

    // Flexbox styling for summary badges
    sensorContainer.style.display = "flex";
    sensorContainer.style.flexWrap = "wrap";
    sensorContainer.style.justifyContent = "flex-end";
    sensorContainer.style.gap = "8px";

    if (checkedSensors.length === 0) {
        sensorContainer.innerHTML = '<span style="opacity:0.5">None selected</span>';
    } else {
        sensorContainer.innerHTML = ''; 
        checkedSensors.forEach(sid => {
            const sObj = data.sensors.find(s => s.id === sid);
            let sName = sObj ? (sObj.name[currentLang] || sObj.name['en'] || sObj.name) : sid;

            let span = document.createElement('span');
            span.textContent = sName;

            // Visual Style: Green Transparent Badge
            span.style.background = "rgba(4, 211, 97, 0.15)";
            span.style.color = "#04d361";
            span.style.padding = "4px 10px";
            span.style.borderRadius = "6px";
            span.style.fontSize = "0.85rem";
            span.style.fontWeight = "600";
            span.style.whiteSpace = "nowrap";

            sensorContainer.appendChild(span);
        });
    }
}

/* * WI-FI SCANNING LOGIC (Polling)
 * Polls the backend until scan results are ready.
 */
function atualizarRedes() {
    const btn = document.querySelector('.icon-btn');
    const icon = btn.querySelector('i');
    const select = document.getElementById('network-select');

    // UI Scanning State
    icon.classList.add('la-spin');
    btn.disabled = true;
    select.innerHTML = `<option value="" disabled selected>Scanning...</option>`;

    // Recursive Polling Function
    const tryScan = (attempts) => {
        fetch('/scan')
            .then(r => r.json())
            .then(data => {
                // If empty array AND attempts < 5, wait and retry
                if ((!data || data.length === 0) && attempts < 5) {
                    console.log("Waiting for scan...", attempts);
                    setTimeout(() => tryScan(attempts + 1), 2000);
                } else {
                    renderList(data);
                }
            })
            .catch(e => {
                console.error(e);
                renderList([]); // Network error
            });
    };

    const renderList = (data) => {
        icon.classList.remove('la-spin');
        btn.disabled = false;

        if (!data || data.length === 0) {
            select.innerHTML = '<option value="" disabled selected>No networks found</option>';
            return;
        }

        select.innerHTML = '<option value="" disabled selected>Select...</option>';
        const unique = new Set();

        data.forEach(n => {
            // Clean up JSON strings
            let cleanN = n.replace(/"/g, '');
            if (!unique.has(cleanN)) {
                unique.add(cleanN);
                let opt = document.createElement('option');
                opt.value = cleanN;
                opt.textContent = cleanN;
                select.appendChild(opt);
            }
        });
    };

    // Start polling
    tryScan(0);
}

function prepareSubmit() {
    const v = Array.from(document.querySelectorAll('.sensor-chk:checked')).map(c => c.value);
    document.getElementById('activeSensors').value = v.join(',');
}

function toggleAdvanced() {
    const s = document.getElementById('show-advanced').checked;
    document.getElementById('advanced-settings').style.display = s ? 'block' : 'none';
}

function checkMachineType() {
    const show = document.getElementById('machineType').value === 'plasma';
    document.getElementById('plasma-specific').style.display = show ? 'block' : 'none';
}

function togglePassword(id) {
    const el = document.getElementById(id);
    const i = el.nextElementSibling.querySelector('i');
    el.type = el.type === 'password' ? 'text' : 'password';
    if (i) i.className = el.type === 'password' ? 'las la-eye' : 'las la-eye-slash';
}

function maskIp(el) {
    el.value = el.value.replace(/[^0-9.]/g, '');
}

function handleFormSubmit(event) {
    event.preventDefault(); // Prevent page reload

    // 1. Prepare Sensor Data
    const v = Array.from(document.querySelectorAll('.sensor-chk:checked')).map(c => c.value);
    document.getElementById('activeSensors').value = v.join(',');

    // 2. UI Feedback
    const btn = document.getElementById('btn-save');
    const originalText = btn.textContent;
    btn.textContent = translations[currentLang].msg_wait || "Wait...";
    btn.disabled = true;

    // 3. Send Data via Fetch
    const form = document.getElementById('mainForm');
    const formData = new FormData(form);

    fetch('/save', {
            method: 'POST',
            body: formData
        })
        .then(response => {
            if (response.ok) {
                showSuccessScreen();
            } else {
                alert("Error saving config.");
                btn.textContent = originalText;
                btn.disabled = false;
            }
        })
        .catch(error => {
            console.error('Error:', error);
            // Assume success on network error (ESP32 reboot interrupts connection)
            showSuccessScreen();
        });
}

function showSuccessScreen() {
    document.getElementById('mainForm').style.display = 'none';
    const card = document.getElementById('success-card');
    card.style.display = 'block';

    // Fade-in animation
    card.style.opacity = '0';
    setTimeout(() => card.style.opacity = '1', 10);
}