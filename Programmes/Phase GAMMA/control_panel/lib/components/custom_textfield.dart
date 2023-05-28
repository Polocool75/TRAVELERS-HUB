import 'package:flutter/material.dart';

class CustomTextField extends StatelessWidget {
  final dynamic controller;
  final String labelText;
  final bool obscureText;

  const CustomTextField({
    Key? key,
    required this.controller,
    required this.labelText,
    required this.obscureText,
  }) : super(key: key);

  @override
  Widget build(BuildContext context) {
    final screenWidth = MediaQuery.of(context).size.width;
    final horizontalPadding = screenWidth.toInt() >= 500 ? 500 : 16;

    return Padding(
      padding: EdgeInsets.symmetric(horizontal: horizontalPadding.toDouble()),
      child: TextField(
        controller: controller,
        obscureText: obscureText,
        decoration: InputDecoration(
          enabledBorder: const OutlineInputBorder(
            borderSide: BorderSide(color: Color.fromARGB(255, 0, 0, 0)),
          ),
          focusedBorder: const OutlineInputBorder(
            borderSide: BorderSide(color: Color(0xFF1331F5)),
          ),
          errorBorder: const OutlineInputBorder(
            borderSide: BorderSide(color: Colors.red),
          ),
          fillColor: const Color.fromARGB(255, 255, 255, 255),
          filled: true,
          labelText: labelText,
        ),
      ),
    );
  }
}

