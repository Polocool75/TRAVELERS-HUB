import 'package:flutter/material.dart';
import 'package:control_panel/components/custom_button.dart';
import 'package:control_panel/components/custom_textfield.dart';

class LoginPage extends StatelessWidget {
  LoginPage({super.key});

  final usernameController = TextEditingController();
  final passwordController = TextEditingController();

  void onSignIn() {
    
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
        body: SafeArea(
            child: Center(
      child: Column(children: [
        const SizedBox(height: 50),
        Image.asset('img/logotype_fond_blanc.png', width: 400, height: 200),
        const Text('Bienvenue sur la plateforme de contrôle de TRAVELERS',
            style: TextStyle(fontSize: 20)),
        const Divider(),
        const SizedBox(height: 50),
        CustomTextField(
            controller: usernameController,
            labelText: 'Email',
            obscureText: false),
        const SizedBox(height: 20),
        CustomTextField(
            controller: passwordController,
            labelText: 'Mot de passe',
            obscureText: true),
        const SizedBox(height: 20),
        const Text('Mot de passe oublié ?'),
        const SizedBox(height: 20),
        CustomButton(
          onTap: onSignIn,
        ),
      ]),
    )));
  }
}
