package main

import "fmt"

func main() {
	var slices = []int{1,0,0}
	slices = append(slices, 100)
	msg := "version "
	fmt.Println(msg, slices)
}