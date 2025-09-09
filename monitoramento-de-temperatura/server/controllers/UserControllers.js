import User from "../models/User.js";
import UserService from "../services/User/UserServices.js";

class UserController {
  constructor() {
    this.userService = new UserService(User);
  }

  createUser = async (req, res) => {
    const userData = req.body;
    const user = await this.userService.createUser(userData);
    res.status(201).json(user);
  }

  getUserByEmail = async (req, res) => {
    const { email } = req.params;
    const user = await this.userService.getUserByEmail(email);
    if (!user) {
      return res.status(404).json({ error: "User not found" });
    }
    res.status(200).json(user);
  };

  getAllUsers = async (req, res) => {
    const users = await this.userService.getAllUsers();
    res.status(200).json(users);
  }

  updateUser = async (req, res) => {
    const { userId } = req.params;
    const updateData = req.body;
    const updatedUser = await this.userService.updateUser(userId, updateData);
    res.status(200).json(updatedUser);
  }

  deleteUser = async (req, res) => {
    const { userId } = req.params;
    await this.userService.deleteUser(userId);
    res.status(204).send();
  }
}

export default UserController;