// Theme Toggle
function toggleTheme() {
    document.body.classList.toggle('dark-mode');
    const btn = document.getElementById('theme-toggle');
    btn.innerText = document.body.classList.contains('dark-mode') ? '☀️' : '🌙';
}

// Navigation
function navigateTo(pageId) {
    document.querySelectorAll('.page').forEach(p => p.classList.remove('active'));
    document.querySelectorAll('.ds-section').forEach(s => s.classList.remove('active'));

    if (pageId === 'landing') {
        document.getElementById('landing-page').classList.add('active');
        document.getElementById('simulation-container').classList.remove('active');
    } else {
        document.getElementById('landing-page').classList.remove('active');
        document.getElementById('simulation-container').classList.add('active');
        document.getElementById(pageId).classList.add('active');

        document.getElementById('svg-layer').innerHTML = '';

        if (pageId === 'stack') stack.render();
        if (pageId === 'queue') queue.render();
        if (pageId === 'sll') sll.render();
        if (pageId === 'dll') dll.render();
    }
}

// Modal
function showModal(text) {
    const modal = document.getElementById('peek-modal');
    document.getElementById('peek-text').innerText = text;
    modal.style.display = 'flex';
}

function closeModal() {
    document.getElementById('peek-modal').style.display = 'none';
}

window.onclick = function (event) {
    const modal = document.getElementById('peek-modal');
    if (event.target == modal) {
        modal.style.display = "none";
    }
}

// Stack Implementation
class Stack {
    constructor() {
        this.items = [];
        this.maxSize = 10;
    }

    push() {
        const input = document.getElementById('stack-input');
        const value = input.value.trim();
        if (!value) return alert("Enter a value");
        if (this.items.length >= this.maxSize) return alert("Stack Overflow");

        this.items.push(value);
        input.value = '';
        this.render();
    }

    pop() {
        if (this.items.length === 0) return alert("Stack Underflow");
        this.items.pop();
        this.render();
    }

    peek() {
        if (this.items.length === 0) return showModal("Stack is empty");
        showModal(`Top Book: ${this.items[this.items.length - 1]}`);
    }

    clear() {
        this.items = [];
        this.render();
    }

    render() {
        const visual = document.getElementById('stack-visual');
        const arrayVisual = document.getElementById('stack-array-visual');
        const topVal = document.getElementById('stack-top-val');

        if (!visual) return;

        visual.innerHTML = '';
        arrayVisual.innerHTML = '';

        topVal.innerText = this.items.length > 0 ? this.items[this.items.length - 1] : "None";

        this.items.forEach(item => {
            const book = document.createElement('div');
            book.className = 'book-item';
            book.innerText = item;
            visual.appendChild(book);
        });

        this.items.forEach(item => {
            const cell = document.createElement('div');
            cell.className = 'array-cell filled';
            cell.innerText = item;
            arrayVisual.appendChild(cell);
        });

        for (let i = this.items.length; i < this.maxSize; i++) {
            const cell = document.createElement('div');
            cell.className = 'array-cell';
            arrayVisual.appendChild(cell);
        }
    }
}

// Queue Implementation
class Queue {
    constructor() {
        this.items = [];
        this.maxSize = 8;
    }

    enqueue() {
        const input = document.getElementById('queue-input');
        const value = input.value.trim();
        if (!value) return alert("Enter a value");
        if (this.items.length >= this.maxSize) return alert("Queue Overflow");

        this.items.push(value);
        input.value = '';
        this.render();
    }

    dequeue() {
        if (this.items.length === 0) return alert("Queue Underflow");
        this.items.shift();
        this.render();
    }

    peek() {
        if (this.items.length === 0) return showModal("Queue is empty");
        showModal(`First Person: ${this.items[0]}`);
    }

    clear() {
        this.items = [];
        this.render();
    }

    render() {
        const visual = document.getElementById('queue-visual');
        const arrayVisual = document.getElementById('queue-array-visual');
        const frontVal = document.getElementById('queue-front-val');
        const rearVal = document.getElementById('queue-rear-val');

        if (!visual) return;

        visual.innerHTML = '';
        arrayVisual.innerHTML = '';

        if (this.items.length > 0) {
            frontVal.innerText = this.items[0];
            rearVal.innerText = this.items[this.items.length - 1];
        } else {
            frontVal.innerText = "None";
            rearVal.innerText = "None";
        }

        this.items.forEach(item => {
            const person = document.createElement('div');
            person.className = 'person-item';

            const head = document.createElement('div');
            head.className = 'person-head';

            const body = document.createElement('div');
            body.className = 'person-body';
            body.innerText = item;

            person.appendChild(head);
            person.appendChild(body);
            visual.appendChild(person);
        });

        this.items.forEach(item => {
            const cell = document.createElement('div');
            cell.className = 'array-cell filled';
            cell.innerText = item;
            arrayVisual.appendChild(cell);
        });

        for (let i = this.items.length; i < this.maxSize; i++) {
            const cell = document.createElement('div');
            cell.className = 'array-cell';
            arrayVisual.appendChild(cell);
        }
    }
}

// Linked List Node
class LLNode {
    constructor(value, x, y) {
        this.value = value;
        this.next = null;
        this.prev = null;
        this.x = x;
        this.y = y;
        this.id = Math.random().toString(36).substr(2, 9);
    }
}

// Draggable Logic
const DraggableMixin = {
    initDrag(node, element, renderCallback) {
        let isDragging = false;
        let startX, startY, initialLeft, initialTop;

        element.addEventListener('mousedown', (e) => {
            isDragging = true;
            startX = e.clientX;
            startY = e.clientY;
            initialLeft = node.x;
            initialTop = node.y;
            element.style.zIndex = 100;
            e.stopPropagation();
        });

        window.addEventListener('mousemove', (e) => {
            if (!isDragging) return;
            const dx = e.clientX - startX;
            const dy = e.clientY - startY;
            node.x = initialLeft + dx;
            node.y = initialTop + dy;
            element.style.left = `${node.x}px`;
            element.style.top = `${node.y}px`;

            renderCallback();
        });

        window.addEventListener('mouseup', () => {
            if (isDragging) {
                isDragging = false;
                element.style.zIndex = 10;
            }
        });
    }
};

// SVG Arrow Drawer with Edge-to-Edge Logic and Vertical Ratios
function drawArrow(svg, startEl, endEl, startSide = 'right', endSide = 'left', offset = 0, startYRatio = 0.5, endYRatio = 0.5) {
    if (!startEl || !endEl) return;

    const startRect = startEl.getBoundingClientRect();
    const endRect = endEl.getBoundingClientRect();
    const containerRect = document.getElementById('simulation-container').getBoundingClientRect();

    let x1, y1, x2, y2;

    // Calculate Start Point
    if (startSide === 'right') {
        x1 = startRect.right - containerRect.left;
        y1 = startRect.top + startRect.height * startYRatio - containerRect.top;
    } else if (startSide === 'left') {
        x1 = startRect.left - containerRect.left;
        y1 = startRect.top + startRect.height * startYRatio - containerRect.top;
    }

    // Calculate End Point
    if (endSide === 'left') {
        x2 = endRect.left - containerRect.left;
        y2 = endRect.top + endRect.height * endYRatio - containerRect.top;
    } else if (endSide === 'right') {
        x2 = endRect.right - containerRect.left;
        y2 = endRect.top + endRect.height * endYRatio - containerRect.top;
    }

    const line = document.createElementNS("http://www.w3.org/2000/svg", "path");

    // Offset control point Y to create curve
    const midX = (x1 + x2) / 2;
    const midY = (y1 + y2) / 2 + offset;

    const path = `M ${x1} ${y1} Q ${midX} ${midY} ${x2} ${y2}`;

    line.setAttribute("d", path);
    line.setAttribute("stroke", getComputedStyle(document.body).getPropertyValue('--text-color').trim());
    line.setAttribute("stroke-width", "2");
    line.setAttribute("fill", "none");
    line.setAttribute("marker-end", "url(#arrowhead)");

    svg.appendChild(line);
}

function setupSVGMarkers() {
    const svg = document.getElementById('svg-layer');
    const color = getComputedStyle(document.body).getPropertyValue('--text-color').trim();
    svg.innerHTML = `
        <defs>
            <marker id="arrowhead" markerWidth="10" markerHeight="7" refX="9" refY="3.5" orient="auto">
                <polygon points="0 0, 10 3.5, 0 7" fill="${color}" />
            </marker>
        </defs>
    `;
    return svg;
}

class SinglyLinkedList {
    constructor() {
        this.head = null;
        this.size = 0;
    }

    add() {
        const input = document.getElementById('sll-input');
        const value = input.value.trim();
        if (!value) return alert("Enter value");

        const x = 50 + this.size * 150;
        const y = 150;

        const newNode = new LLNode(value, x, y);

        if (!this.head) {
            this.head = newNode;
        } else {
            let current = this.head;
            while (current.next) {
                current = current.next;
            }
            current.next = newNode;
        }
        this.size++;
        input.value = '';
        this.render();
    }

    insert() {
        const inputVal = document.getElementById('sll-input');
        const inputIdx = document.getElementById('sll-index');
        const value = inputVal.value.trim();
        const index = parseInt(inputIdx.value);

        if (!value) return alert("Enter value");
        if (isNaN(index) || index < 0 || index > this.size) return alert("Invalid index");

        const x = 50 + index * 150;
        const y = 250;
        const newNode = new LLNode(value, x, y);

        if (index === 0) {
            newNode.next = this.head;
            this.head = newNode;
        } else {
            let current = this.head;
            let i = 0;
            while (i < index - 1) {
                current = current.next;
                i++;
            }
            newNode.next = current.next;
            current.next = newNode;
        }
        this.size++;
        inputVal.value = '';
        inputIdx.value = '';
        this.render();
    }

    remove() {
        const inputIdx = document.getElementById('sll-index');
        const index = parseInt(inputIdx.value);
        if (isNaN(index) || index < 0 || index >= this.size) return alert("Invalid index");

        if (index === 0) {
            this.head = this.head.next;
        } else {
            let current = this.head;
            let i = 0;
            while (i < index - 1) {
                current = current.next;
                i++;
            }
            current.next = current.next.next;
        }
        this.size--;
        inputIdx.value = '';
        this.render();
    }

    clear() {
        this.head = null;
        this.size = 0;
        this.render();
    }

    render() {
        const container = document.getElementById('sll-canvas');
        container.innerHTML = '';

        let current = this.head;
        while (current) {
            const el = document.createElement('div');
            el.className = 'draggable-node sll-node-content';
            el.id = `node-${current.id}`;
            el.style.left = `${current.x}px`;
            el.style.top = `${current.y}px`;

            const nextContent = current.next ? 'Next' : '<span class="null-ptr">✕</span>';

            el.innerHTML = `
                <div class="node-data">${current.value}</div>
                <div class="node-ptr node-ptr-next">${nextContent}</div>
            `;

            container.appendChild(el);
            DraggableMixin.initDrag(current, el, () => this.drawArrows());
            current = current.next;
        }

        this.drawArrows();
    }

    drawArrows() {
        const svg = setupSVGMarkers();
        let current = this.head;

        while (current && current.next) {
            const startNode = document.getElementById(`node-${current.id}`);
            const endNode = document.getElementById(`node-${current.next.id}`);

            if (startNode && endNode) {
                const startPtr = startNode.querySelector('.node-ptr-next');
                // SLL: Default ratio 0.5 (Center)
                drawArrow(svg, startPtr, endNode, 'right', 'left', 0, 0.5, 0.5);
            }
            current = current.next;
        }
    }
}

class DoublyLinkedList {
    constructor() {
        this.head = null;
        this.size = 0;
    }

    add() {
        const input = document.getElementById('dll-input');
        const value = input.value.trim();
        if (!value) return alert("Enter value");

        const x = 50 + this.size * 180;
        const y = 150;

        const newNode = new LLNode(value, x, y);

        if (!this.head) {
            this.head = newNode;
        } else {
            let current = this.head;
            while (current.next) {
                current = current.next;
            }
            current.next = newNode;
            newNode.prev = current;
        }
        this.size++;
        input.value = '';
        this.render();
    }

    insert() {
        const inputVal = document.getElementById('dll-input');
        const inputIdx = document.getElementById('dll-index');
        const value = inputVal.value.trim();
        const index = parseInt(inputIdx.value);

        if (!value) return alert("Enter value");
        if (isNaN(index) || index < 0 || index > this.size) return alert("Invalid index");

        const x = 50 + index * 180;
        const y = 250;
        const newNode = new LLNode(value, x, y);

        if (index === 0) {
            if (this.head) {
                newNode.next = this.head;
                this.head.prev = newNode;
            }
            this.head = newNode;
        } else {
            let current = this.head;
            let i = 0;
            while (i < index - 1) {
                current = current.next;
                i++;
            }
            newNode.next = current.next;
            newNode.prev = current;
            if (current.next) {
                current.next.prev = newNode;
            }
            current.next = newNode;
        }
        this.size++;
        inputVal.value = '';
        inputIdx.value = '';
        this.render();
    }

    remove() {
        const inputIdx = document.getElementById('dll-index');
        const index = parseInt(inputIdx.value);
        if (isNaN(index) || index < 0 || index >= this.size) return alert("Invalid index");

        if (index === 0) {
            this.head = this.head.next;
            if (this.head) this.head.prev = null;
        } else {
            let current = this.head;
            let i = 0;
            while (i < index) {
                current = current.next;
                i++;
            }
            if (current.prev) current.prev.next = current.next;
            if (current.next) current.next.prev = current.prev;
        }
        this.size--;
        inputIdx.value = '';
        this.render();
    }

    clear() {
        this.head = null;
        this.size = 0;
        this.render();
    }

    render() {
        const container = document.getElementById('dll-canvas');
        container.innerHTML = '';

        let current = this.head;
        while (current) {
            const el = document.createElement('div');
            el.className = 'draggable-node dll-node-content';
            el.id = `node-${current.id}`;
            el.style.left = `${current.x}px`;
            el.style.top = `${current.y}px`;

            const prevContent = current.prev ? 'Prev' : '<span class="null-ptr">✕</span>';
            const nextContent = current.next ? 'Next' : '<span class="null-ptr">✕</span>';

            el.innerHTML = `
                <div class="node-ptr-prev">${prevContent}</div>
                <div class="node-data">${current.value}</div>
                <div class="node-ptr-next">${nextContent}</div>
            `;

            container.appendChild(el);
            DraggableMixin.initDrag(current, el, () => this.drawArrows());
            current = current.next;
        }
        this.drawArrows();
    }

    drawArrows() {
        const svg = setupSVGMarkers();
        let current = this.head;

        while (current && current.next) {
            const startNode = document.getElementById(`node-${current.id}`);
            const endNode = document.getElementById(`node-${current.next.id}`);

            if (startNode && endNode) {
                const startNext = startNode.querySelector('.node-ptr-next');
                const endPrev = endNode.querySelector('.node-ptr-prev');

                // Forward: Right of Next Ptr -> Left of Prev Ptr
                // Top Quarter (0.25)
                drawArrow(svg, startNext, endPrev, 'right', 'left', 10, 0.25, 0.25);

                // Backward: Left of Prev Ptr -> Right of Next Ptr
                // Bottom Quarter (0.75)
                drawArrow(svg, endPrev, startNext, 'left', 'right', -10, 0.75, 0.75);
            }
            current = current.next;
        }
    }
}

const stack = new Stack();
const queue = new Queue();
const sll = new SinglyLinkedList();
const dll = new DoublyLinkedList();

setupSVGMarkers();
navigateTo('landing');
