package main

import (
	"encoding/json"
	"fmt"
	"html/template"
	"io/ioutil"
	"net/http"
)

// Struct to hold data for rendering the HTML template
type PageData struct {
	Title string
}

// handler function for the web root "/"
func homePage(w http.ResponseWriter, r *http.Request) {
	switch r.Method {
	case "GET":
		// Respond to GET requests
		tmpl := template.Must(template.ParseFiles("index.html"))
		data := PageData{Title: "Go Web App"}
		tmpl.Execute(w, data)
	case "POST":
		// Parse the request body based on content type
		contentType := r.Header.Get("Content-Type")
		switch contentType {
		case "application/x-www-form-urlencoded":
			// Parse form data
			err := r.ParseForm()
			if err != nil {
				http.Error(w, "Failed to parse form data", http.StatusBadRequest)
				return
			}
			name := r.Form.Get("name")
			fmt.Fprintf(w, "Hello, %s! This is a POST request with form data.", name)
		case "application/json":
			// Parse JSON data
			body, err := ioutil.ReadAll(r.Body)
			if err != nil {
				http.Error(w, "Failed to read request body", http.StatusBadRequest)
				return
			}
			var data map[string]string
			err = json.Unmarshal(body, &data)
			if err != nil {
				http.Error(w, "Failed to parse JSON data", http.StatusBadRequest)
				return
			}
			name := data["name"]
			fmt.Fprintf(w, "Hello, %s! This is a POST request with JSON data.", name)
		default:
			http.Error(w, "Unsupported content type", http.StatusUnsupportedMediaType)
		}
	default:
		// Respond with a 405 Method Not Allowed error for other request methods
		w.WriteHeader(http.StatusMethodNotAllowed)
		fmt.Fprintf(w, "Unsupported request method.")
	}
}

func main() {
	// Registering the handler function for the root URL
	http.HandleFunc("/", homePage)

	// Start the HTTP server on port 8080
	fmt.Println("Server listening on port 8080...")
	err := http.ListenAndServe(":8080", nil)
	if err != nil {
		fmt.Println("Error starting server:", err)
	}
}
